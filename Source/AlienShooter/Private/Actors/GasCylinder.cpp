// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/GasCylinder.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Damage/ExplosionDamage.h"

AGasCylinder::AGasCylinder()
{
	GasCylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosiveMesh"));
	GasCylinderMesh->SetupAttachment(GetRootComponent());

	ExplosionOriginPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionOriginPoint"));
	ExplosionOriginPoint->SetupAttachment(GasCylinderMesh);
}

void AGasCylinder::BeginPlay()
{
}

float AGasCylinder::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser)
{
	SpawnEffects();
	ApplyDamage();
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AGasCylinder::DestroyCallback, DestroyDelay, false, DestroyDelay);

	return DamageProperties.BaseDamage;
}

void AGasCylinder::SpawnEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticleSystem, ExplosionOriginPoint->GetComponentLocation(),
		ExplosionOriginPoint->GetComponentRotation(), true, EPSCPoolMethod::None, true);
}

void AGasCylinder::ApplyDamage()
{
	TArray<AActor*> ActorsToIgnore;
	UGameplayStatics::ApplyRadialDamage(this, DamageProperties.BaseDamage,
		ExplosionOriginPoint->GetComponentLocation(), DamageProperties.DamageInnerRadius,
		UExplosionDamage::StaticClass(), ActorsToIgnore);
}

void AGasCylinder::DestroyCallback()
{
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	GasCylinderMesh->SetSimulatePhysics(false);
	GasCylinderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GasCylinderMesh->DestroyComponent();
	ExplosionOriginPoint->DestroyComponent();
	Destroy();
	if (!IsValid(this))
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("Actor is pending kill or invalid."));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("Actor is valid"));
}

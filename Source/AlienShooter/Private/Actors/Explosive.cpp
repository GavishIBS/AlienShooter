// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.



#include "Actors/Explosive.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Damage/ExplosionDamage.h"

AExplosive::AExplosive()
{
	PrimaryActorTick.bCanEverTick = false;

	ExplosiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosiveMesh"));
	ExplosiveMesh->SetupAttachment(GetRootComponent());

	ExplosionOriginPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionOriginPoint"));
	ExplosionOriginPoint->SetupAttachment(ExplosiveMesh);
}

void AExplosive::BeginPlay()
{
}

float AExplosive::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser)
{
	// Spawn effects
	UNiagaraSystem* NiagaraSystem = nullptr;
	if (!ExplosionParticleSystem.Get())
		ExplosionParticleSystem.LoadSynchronous();
	NiagaraSystem = ExplosionParticleSystem.Get();
	if (NiagaraSystem)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(),
			GetActorRotation(), GetActorScale3D(), true, true);

	TArray<AActor*> ActorsToIgnore;
	//UGameplayStatics::ApplyRadialDamageWithFalloff(this, DamageProperties.BaseDamage,
		//DamageProperties.MinimumDamage, ExplosionOriginPoint->GetComponentLocation(),
		//DamageProperties.DamageInnerRadius, DamageProperties.DamageOuterRadius,
		//DamageProperties.DamageFalloff, UExplosionDamage::StaticClass(), ActorsToIgnore, InDamageCauser);
	UGameplayStatics::ApplyRadialDamage(this, DamageProperties.BaseDamage,
		ExplosionOriginPoint->GetComponentLocation(), DamageProperties.DamageInnerRadius,
		UExplosionDamage::StaticClass(), ActorsToIgnore);

	// Destroy
	ExplosiveMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ExplosiveMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Destroy();

	return 100000.0f;
}

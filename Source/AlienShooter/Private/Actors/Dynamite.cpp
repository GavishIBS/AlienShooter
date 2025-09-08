// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Dynamite.h"

ADynamite::ADynamite()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADynamite::Explode(AActor* ThisObject, UParticleSystem* InExplosionParticleSystem, const FDamageProperties& InDamageProperties,
	const FVector& Loc, const FRotator& Rot, const TArray<AActor*> InActorsToIgnore)
{
	IExplodeInterface::Explode(ThisObject, InExplosionParticleSystem, InDamageProperties, Loc, Rot, InActorsToIgnore);
}

void ADynamite::BeginPlay()
{
	Super::BeginPlay();
}

void ADynamite::PlantDynamiteAtLocation(const FVector& Location, const FRotator& Rotation)
{
	SetActorLocation(Location);
	SetActorRotation(Rotation);
	StartExplosionTimer();
}

void ADynamite::StartExplosionTimer()
{
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ADynamite::ExplodeWrapper, ExplosionDelay, false, ExplosionDelay);
}

void ADynamite::ExplodeWrapper()
{
	/*
	if (ExplosionParticleSystem)
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticleSystem, GetActorLocation(), GetActorRotation(), true);

	UGameplayStatics::ApplyRadialDamage(this, DamageProperties.BaseDamage,
		Mesh->GetComponentLocation(), DamageProperties.DamageInnerRadius,
		UExplosionDamage::StaticClass(), TArray<AActor*>());

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("Explosion"));
	*/

	Explode(this, ExplosionParticleSystem, DamageProperties, GetActorLocation(), GetActorRotation(), TArray<AActor*>());
	Destroy();
}

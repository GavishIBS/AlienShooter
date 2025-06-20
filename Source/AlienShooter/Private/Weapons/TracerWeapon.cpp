// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/TracerWeapon.h"
#include "Kismet/GameplayStatics.h"

ATracerWeapon::ATracerWeapon()
{
}

void ATracerWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ATracerWeapon::Fire()
{
	Super::Fire();
}

void ATracerWeapon::OnBulletHit(AActor* HitActor, const FVector& ImpactPoint, UPhysicalMaterial* PhysMat)
{
	if (!HitActor) return;

	if (PhysMat) {
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMat);

		if (TracerWeaponData.HitEffects.Contains(SurfaceType)) {
			const FBulletHitEffectData& EffectData = TracerWeaponData.HitEffects[SurfaceType];

			if (EffectData.HitParticle)
				UGameplayStatics::SpawnEmitterAtLocation(this, EffectData.HitParticle, ImpactPoint);

			if (EffectData.HitSound)
				UGameplayStatics::PlaySoundAtLocation(this, EffectData.HitSound, ImpactPoint);
		}
	}

	/*
	TArray<FName> ActorTags = HitActor->Tags;
	for (auto It = TracerWeaponData.HitEffects.CreateIterator(); It; ++It)
	{
		// Check if the tag exists directly in the ActorTags array
		if (ActorTags.Contains(It.Key()))
		{
			// Spawn particle if the value exists
			if (It.Value().HitParticle)
				UGameplayStatics::SpawnEmitterAtLocation(this, It.Value().HitParticle, ImpactPoint);
			if (It.Value().HitSound)
				UGameplayStatics::SpawnSoundAtLocation(this, It.Value().HitSound, ImpactPoint, FRotator());
			return; // Exit the loop
		}
	}
	*/
}

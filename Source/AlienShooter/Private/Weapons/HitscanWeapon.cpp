// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/HitscanWeapon.h"
#include "Characters/ShooterCharacter.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AHitscanWeapon::AHitscanWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHitscanWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AHitscanWeapon::Fire()
{
	Super::Fire();

	if (!OwnerAsShooterCharacter) return;

	float BulletHealth = WeaponProperties.BulletHealth;
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(OwnerAsShooterCharacter);
	for (int i = 0; i < HitscanWeaponProperties.NumberOfTraces; i++)
	{
		const float TraceHeight = HitscanWeaponProperties.TraceHeightOffset +
			OwnerAsShooterCharacter->GetMesh()->GetComponentLocation().Z -
				((i * OwnerAsShooterCharacter->GetMesh()->GetRelativeLocation().Z) / HitscanWeaponProperties.NumberOfHeightDivisions);

		const FVector MuzzleLocation = GetWeaponMuzzleTransform().GetLocation();
		const FVector Start = FVector(MuzzleLocation.X, MuzzleLocation.Y, TraceHeight);
		const FVector End = Start + MuzzleDirection->GetForwardVector() * HitscanWeaponProperties.Range;

		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingle(this, Start, End, 3.f, TraceTypeQuery1,
			false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

		if (HitResult.bBlockingHit)
		{
			if (IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor()))
			{
				EGetHitReturnType OutGetHitReturn;
				BulletHealth -= HitInterface->GetHit(HitResult.ImpactPoint, WeaponProperties.Damage, WeaponProperties.Damage_RandomizationFactor, OwnerAsShooterCharacter, UDamageType::StaticClass(), &OutGetHitReturn);
				if (OutGetHitReturn == EGetHitReturnType::MonsterKill)
				{
					OnMonsterKill.Broadcast();
				}
			}
			
			// Hit particles
			OnBulletHit(HitResult.GetActor(), HitResult.ImpactPoint, HitResult.PhysMaterial.Get());
			if (HitResult.GetActor())
			{
				/*
				TArray<FName> ActorTags = HitResult.GetActor()->Tags;
				ActorTags.Sort();
				for (auto It = WeaponFXData.HitParticles.CreateIterator(); It; ++It)
				{
					// Use binary search to check if the tag exists in ActorTags
					if (Algo::BinarySearch(ActorTags, It.Key()))
					{
						// Spawn particle if the value exists
						if (It.Value())
							UGameplayStatics::SpawnEmitterAtLocation(this, It.Value(), HitResult.ImpactPoint);
						goto END_LOOP; // Exit the loop
					}
				}
				*/
				/*
				TArray<FName> ActorTags = HitResult.GetActor()->Tags;
				for (auto It = WeaponFXData.HitParticles.CreateIterator(); It; ++It)
				{
					// Check if the tag exists directly in the ActorTags array
					if (ActorTags.Contains(It.Key()))
					{
						// Spawn particle if the value exists
						if (It.Value())
							UGameplayStatics::SpawnEmitterAtLocation(this, It.Value(), HitResult.ImpactPoint);
						goto END_LOOP; // Exit the loop
					}
				}
				*/
			}

			//END_LOOP:

			if (BulletHealth <= 0.f)
				break;

			// TODO: the below break; is breaking the loop even if the bullet health is more than 0.
			break;
		}
	}
}

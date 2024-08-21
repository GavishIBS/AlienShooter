// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/HitscanWeapon.h"
#include "Characters/ShooterCharacter.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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
				BulletHealth -= HitInterface->GetHit(HitResult.ImpactPoint, WeaponProperties.Damage, WeaponProperties.Damage_RandomizationFactor, OwnerAsShooterCharacter);
			if (BulletHealth <= 0.f)
				break;
		}
	}
}

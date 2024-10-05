// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/CursorWeapon.h"
#include "GameFramework/Actor.h"
#include "Characters/ShooterCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// DEBUG
#include "DrawDebugHelpers.h"

ACursorWeapon::ACursorWeapon()
{
}

void ACursorWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ACursorWeapon::Fire()
{
	Super::Fire();

	if (!OwnerAsShooterCharacter || !OwnerAsShooterCharacter->GetPlayerController()) return;

	float BulletHealth = WeaponProperties.BulletHealth;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(OwnerAsShooterCharacter);
	
	FHitResult HitResult;
	OwnerAsShooterCharacter->GetPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (HitResult.bBlockingHit)
	{
		if (IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor()))
		{
			BulletHealth -= HitInterface->GetHit(HitResult.ImpactPoint, WeaponProperties.Damage, WeaponProperties.Damage_RandomizationFactor, OwnerAsShooterCharacter);

			// DEBUG
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.f, 24, FColor::Green, false, 4.f);
		}
		OnBulletHit(HitResult.GetActor(), HitResult.ImpactPoint);
		if (BulletHealth <= 0.f) {}
	}
}

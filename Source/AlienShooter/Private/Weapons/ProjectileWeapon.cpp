// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/ProjectileWeapon.h"
#include "Weapons/Projectile.h"
#include "Characters/ShooterCharacter.h"

AProjectileWeapon::AProjectileWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileWeapon::Fire()
{
	Super::Fire();
	
	if (GetWorld() && OwnerAsShooterCharacter)
	{
		const FTransform MuzzleTransform = GetWeaponMuzzleTransform();
		checkf(ProjectileWeaponData.ProjectileClass, TEXT("Projectile class is not set!"));
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Owner = this;
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileWeaponData.ProjectileClass, MuzzleTransform, ActorSpawnParameters);
		//Projectile->Init(WeaponProperties.BulletHealth, WeaponProperties.Damage, WeaponProperties.Damage_RandomizationFactor);
		Projectile->WeaponOwner = (AActor*)(OwnerAsShooterCharacter);

		if (WeaponProperties.Accuracy >= FMath::RandRange(1, 100))
		{
			Projectile->SetTargetLocation(OwnerAsShooterCharacter->GetCursorTraceHitLocation());
		} else {
			FVector ProjectileTargetLocation = OwnerAsShooterCharacter->GetCursorTraceHitLocation();
			ProjectileTargetLocation.X += FMath::RandRange(-WeaponProperties.HitLocation_RandomizationAdditiveRange.X,
				WeaponProperties.HitLocation_RandomizationAdditiveRange.X);
			ProjectileTargetLocation.Y += FMath::RandRange(-WeaponProperties.HitLocation_RandomizationAdditiveRange.Y,
				WeaponProperties.HitLocation_RandomizationAdditiveRange.Y);
			Projectile->SetTargetLocation(ProjectileTargetLocation);
		}
	}
}

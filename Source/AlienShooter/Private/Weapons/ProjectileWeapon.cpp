// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/ProjectileWeapon.h"
#include "Weapons/Projectile.h"

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
	
	if (GetWorld())
	{
		const FTransform MuzzleTransform = GetWeaponMuzzleTransform();
		checkf(ProjectileWeaponData.ProjectileClass, TEXT("Projectile class is not set!"));
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Owner = this;
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileWeaponData.ProjectileClass, MuzzleTransform, ActorSpawnParameters);
		Projectile->Init(WeaponProperties.BulletHealth, WeaponProperties.Damage, WeaponProperties.Damage_RandomizationFactor);
		Projectile->WeaponOwner = (AActor*)(OwnerAsShooterCharacter);
	}
}

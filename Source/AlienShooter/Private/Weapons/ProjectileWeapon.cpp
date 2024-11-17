// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/ProjectileWeapon.h"
#include "Weapons/Projectile.h"
#include "Characters/ShooterCharacter.h"
#include "ObjectPool.h"

AProjectileWeapon::AProjectileWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileWeapon::Init_Callbacks()
{
	Super::Init_Callbacks();
}

void AProjectileWeapon::OnWeaponSelectedCallback()
{
	Super::OnWeaponSelectedCallback();

	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, FString("Pool created!"));

	// TODO: Destory pool if ammo is exhausted but weapon is still in hand

	int PoolSize = ProjectilePoolSize;
	if (PoolSize > GetTotalAmmoAvailable())
		PoolSize = GetTotalAmmoAvailable();

	bool bCreateProjectilePoolComponent = true;
	if (PoolSize <= 0)
		bCreateProjectilePoolComponent = false;

	if (bCreateProjectilePoolComponent)
	{
		ProjectilePool = new UObjectPool<AProjectile>(GetWorld(), ProjectileWeaponData.ProjectileClass, PoolSize);
		int ProjectilePoolValid = ProjectilePool->CreatePool();
		if (ProjectilePoolValid <= 0)
			delete ProjectilePool;
	}
}

void AProjectileWeapon::OnWeaponDestroyed(AActor* DestroyedActor)
{
	Super::OnWeaponDestroyed(DestroyedActor);
	if (ProjectilePool != nullptr)
	{
		ProjectilePool->DestroyPool();
		delete ProjectilePool;
		ProjectilePool = nullptr;
	}
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

		AProjectile* Projectile = nullptr;
		if (ProjectilePool != nullptr)
		{
			// Use projectile from pool
			Projectile = ProjectilePool->SpawnObjectFromPool();
			if (Projectile)
			{
				GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, FString("Spawned from pool!"));
				Projectile->SetActorLocation(MuzzleTransform.GetLocation());
				Projectile->SetActorRotation(MuzzleTransform.GetRotation());
				Projectile->SetInUse(true);
			}
		}
		if (Projectile == nullptr) {
			// Spawn projectile if not available from pool
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, FString("Spawned directly!"));
			Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileWeaponData.ProjectileClass, MuzzleTransform, ActorSpawnParameters);
		}
		
		if (Projectile)
		{
			//Projectile->Init(WeaponProperties.BulletHealth, WeaponProperties.Damage, WeaponProperties.Damage_RandomizationFactor);
			Projectile->WeaponOwner = (AActor*)(OwnerAsShooterCharacter);  // TODO: Check if Projectile is valid

			if (WeaponProperties.Accuracy >= FMath::RandRange(1, 100))
			{
				Projectile->SetTargetLocation(OwnerAsShooterCharacter->GetCursorTraceHitLocation());
			}
			else {
				FVector ProjectileTargetLocation = OwnerAsShooterCharacter->GetCursorTraceHitLocation();
				ProjectileTargetLocation.X += FMath::RandRange(-WeaponProperties.HitLocation_RandomizationAdditiveRange.X,
					WeaponProperties.HitLocation_RandomizationAdditiveRange.X);
				ProjectileTargetLocation.Y += FMath::RandRange(-WeaponProperties.HitLocation_RandomizationAdditiveRange.Y,
					WeaponProperties.HitLocation_RandomizationAdditiveRange.Y);
				Projectile->SetTargetLocation(ProjectileTargetLocation);
			}
		}
	}
}

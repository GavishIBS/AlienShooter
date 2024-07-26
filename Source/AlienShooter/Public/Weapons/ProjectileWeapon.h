// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AProjectileWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AProjectileWeapon();

protected:
	virtual void BeginPlay() override;

	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile | Properties")
	FProjectileWeaponProperties ProjectileWeaponProperties = FProjectileWeaponProperties();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile | Data")
	FProjectileWeaponData ProjectileWeaponData = FProjectileWeaponData();
};

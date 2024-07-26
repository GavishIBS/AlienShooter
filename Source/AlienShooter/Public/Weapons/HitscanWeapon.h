// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "HitscanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AHitscanWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AHitscanWeapon();

protected:
	virtual void BeginPlay() override;

	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hitscan | Properties")
	FHitscanWeaponProperties HitscanWeaponProperties = FHitscanWeaponProperties();
};

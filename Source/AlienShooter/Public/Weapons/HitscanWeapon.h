// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TracerWeapon.h"
#include "HitscanWeapon.generated.h"

/**
 * Hitscan weapons have inbuilt or complete authority over bullets that's why it derives from IBulletInterface
 */
UCLASS()
class ALIENSHOOTER_API AHitscanWeapon : public ATracerWeapon
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

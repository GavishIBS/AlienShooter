// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "CursorWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ACursorWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	ACursorWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Fire() override;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TracerWeapon.h"
#include "CursorWeapon.generated.h"

/**
 * Hitscan weapons have inbuilt or complete authority over bullets that's why it derives from IBulletInterface
 */
UCLASS()
class ALIENSHOOTER_API ACursorWeapon : public ATracerWeapon
{
	GENERATED_BODY()

public:
	ACursorWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Fire() override;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (ClampMin = "0"))
	float DistanceToInBetweenHit = 40.f;
};

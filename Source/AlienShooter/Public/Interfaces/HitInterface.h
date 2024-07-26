// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALIENSHOOTER_API IHitInterface
{
	GENERATED_BODY()

public:
	virtual float GetHit(const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser);
};

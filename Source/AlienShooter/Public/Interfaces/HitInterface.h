// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

struct FSpecialEffectData;

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
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser);
	virtual void GetSpecialHit(const FSpecialEffectData& SpecialEffectData);   // TODO: Instead of directly passing the effect data, pass a tag which fetches the data from a global data pool
};

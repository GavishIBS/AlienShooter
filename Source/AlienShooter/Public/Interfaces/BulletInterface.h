// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BulletInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBulletInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALIENSHOOTER_API IBulletInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnBulletHit(AActor* HitActor, const FVector& ImpactPoint) = 0;
};

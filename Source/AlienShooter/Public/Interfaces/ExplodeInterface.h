// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Utilities.h"
#include "ExplodeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UExplodeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALIENSHOOTER_API IExplodeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Explode(const FDamageProperties& DamageProperties);
};

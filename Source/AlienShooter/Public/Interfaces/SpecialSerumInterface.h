// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpecialSerumInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpecialSerumInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALIENSHOOTER_API ISpecialSerumInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	ISpecialSerumInterface() = default;
	
	//virtual bool ApplyEffect(ACharacter* InCharacter);
	//virtual struct FSpecialSerumEffectData GetSerumEffectsData() const;
};

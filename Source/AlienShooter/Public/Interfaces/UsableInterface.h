// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UsableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUsableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALIENSHOOTER_API IUsableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual class AUsableActor* UseItem(ACharacter* User) = 0;
	virtual void OnItemUsed();
};

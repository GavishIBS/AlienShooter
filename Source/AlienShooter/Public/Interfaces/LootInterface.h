// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LootInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULootInterface : public UInterface
{
	GENERATED_BODY()
};

struct FLootData;

/**
 * 
 */
class ALIENSHOOTER_API ILootInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetLootData(const FLootData& InLootData);
	virtual void SpawnLoot();
};

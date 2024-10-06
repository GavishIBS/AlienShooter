// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Utils/LootManager.h"
#include "LootTypes.h"
#include "ItemLootManager.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AItemLootManager : public ALootManager
{
	GENERATED_BODY()
	
public:
	AItemLootManager();

protected:
	virtual void AddLoot() override;

	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<TSubclassOf<AActor>, FLootData> Items_LootData;
};

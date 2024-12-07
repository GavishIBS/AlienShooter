// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LootTypes.generated.h"

USTRUCT()
struct FLootData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Loot Data")
	TSubclassOf<AActor> LootItemClass = nullptr;

	UPROPERTY(EdiTAnywhere, Category="Loot Data")
	int Quantity = 1;
};

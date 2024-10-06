// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include "Gameframework/Actor.h"
#include "../LootTypes.h"
#include "LootManager.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ALootManager : public AActor
{
	GENERATED_BODY()
	
public:
	ALootManager();

private:
	UFUNCTION(CallInEditor, Category = "Functions")
	void AddLoot();

	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<TSubclassOf<AActor>, FLootData> Items_LootData;
};

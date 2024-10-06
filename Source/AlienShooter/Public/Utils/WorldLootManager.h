// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Utils/LootManager.h"
#include "LootTypes.h"
#include "WorldLootManager.generated.h"

class ALootTargetPoint;

UCLASS()
class ALIENSHOOTER_API AWorldLootManager : public ALootManager
{
	GENERATED_BODY()
	
public:	
	AWorldLootManager();

protected:
	virtual void AddLoot() override;

	UPROPERTY(EditAnywhere, Category = "Data | Loot")
	TArray<FLootData> World_LootData;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<ALootTargetPoint> TargetPointClass = nullptr;

private:
	UFUNCTION(CallInEditor, Category = "Functions")
	void ClearLoot();

	TArray<AActor*> SpawnedLoot_Cache;
};

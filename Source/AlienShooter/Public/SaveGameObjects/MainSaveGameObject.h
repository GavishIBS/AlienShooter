// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../InventoryTypes.h"
#include "MainSaveGameObject.generated.h"

class AWeaponBase;

/**
 * PlayerDataSaveObject - stores all the data which is directly related to player
 */
UCLASS()
class ALIENSHOOTER_API UMainSaveGameObject : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMainSaveGameObject();

	UPROPERTY(VisibleAnywhere, Category = "Data | Player | Details")
	FString PlayerName = FString("PlayerName");

	UPROPERTY(VisibleAnywhere, Category = "Data | Player | Equipment | Weapons")
	TMap<TSubclassOf<AWeaponBase>, int> Weapons;

	UPROPERTY(VisibleAnywhere, Category = "Data | Player | Equipment | Inventory")
	TArray<FSlotData> InventoryContent;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
};

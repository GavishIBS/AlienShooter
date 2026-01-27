// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameDataSaveObject.generated.h"

/**
 * stores data about game itself, such as number of levels completed so far
 */
UCLASS()
class ALIENSHOOTER_API UGameDataSaveObject : public USaveGame
{
	GENERATED_BODY()
	
public:
	UGameDataSaveObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data | Player | Details")
	int32 NumberOfLevelsCompleted = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data | Game")
	int32 NumGasCanisters = 0;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
};

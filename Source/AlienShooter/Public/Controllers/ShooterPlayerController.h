// // Copyright (c) 2024 Gavish Bansal// // All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AShooterPlayerController();

protected:
	UFUNCTION(BlueprintCallable)   // TEMP // DEBUG
	bool SavePlayerData();
	UFUNCTION(BlueprintCallable)   // TEMP // DEBUG
	bool LoadPlayerData();
};

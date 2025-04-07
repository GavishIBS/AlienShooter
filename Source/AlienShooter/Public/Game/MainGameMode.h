// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category="Game | Rebirth")
	FTransform LastCheckpointTransform;

protected:
	ABaseCharacter* BChar;

protected:
	//virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Game | Data", meta = (ClampMin="0", ClampMax="9"))
	int32 RespawnCount = 3;

	UFUNCTION()
	void TryRespawnPlayer(AActor* Killer);

public:
	//UFUNCTION(BlueprintCallable, Category = "Game | Rebirth")
	FORCEINLINE bool CanRespawn() const { return RespawnCount > 0 && BChar; }
	
	//UFUNCTION(BlueprintCallable, Category = "Game | Rebirth")
	void RespawnPlayer();
};

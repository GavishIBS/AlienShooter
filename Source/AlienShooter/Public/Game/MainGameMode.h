// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AlarmingTypes.h"
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

	/** Alarming system begins */
	UFUNCTION(BlueprintCallable)   // TEST // DEBUG
	void EnableAlarmingSystem(const EAlarmingModeName& SwitchToModeName = EAlarmingModeName::NORMAL);
	UFUNCTION(BlueprintCallable)   // TEST // DEBUG
	void DisableAlarmingSystem();
	UFUNCTION(BlueprintCallable)   // TEST // DEBUG
	void ChangeAlarmingMode(const EAlarmingModeName& SwitchToModeName = EAlarmingModeName::NORMAL);

	//UFUNCTION()
	virtual void OnAlarmingModeChanged(const EAlarmingModeName& NewModeName, const EAlarmingModeName& LastModeName);

	EAlarmingModeName CurrentAlarmingModeName = EAlarmingModeName::NONE;
	class UAudioComponent* CurrentAlarmModeSound_AudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data | AlarmingSystem")
	TArray<FAlarmingModeData> AlarmingModes;          // 4 modes - normal, safe, gray-zone, and danger

	void Setup_AlarmLights();
	TArray<class AAlarmDirectionalLight_Electric*> Alarm_DirectionalLights;

private:
	FAlarmingModeData GetAlarmingModeData_ByName(const EAlarmingModeName& ModeName);
	/** Alarming system ends */

public:
	//UFUNCTION(BlueprintCallable, Category = "Game | Rebirth")
	FORCEINLINE bool CanRespawn() const { return RespawnCount > 0 && BChar; }
	
	//UFUNCTION(BlueprintCallable, Category = "Game | Rebirth")
	void RespawnPlayer();
};

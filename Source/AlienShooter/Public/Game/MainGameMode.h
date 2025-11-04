// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AlarmingTypes.h"
#include "MainGameMode.generated.h"

class ABaseCharacter;
class AElectric_Siren_SpotLight;
class USoundMix;
class USoundClass;
class AElectronicDoor;
class AAISpawnerBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAlarmingModeChanged, EAlarmingModeName, NewModeName, EAlarmingModeName, LastModeName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlaySequence, float, Duration);

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnPlaySequence OnPlaySequence;

	UFUNCTION(BlueprintCallable)
	void SkipSequence();

public:
	UPROPERTY(BlueprintReadWrite, Category="Game | Rebirth")
	FTransform LastCheckpointTransform;

protected:
	ABaseCharacter* BChar;

	UFUNCTION()
	void OnSequencePlayed(float Duration);

	void EndSequence();

	UPROPERTY(EditDefaultsOnly, Category = "Game | Data", meta = (ClampMin = "0", ClampMax = "9"))
	float ControlToggleOnSequence_Delay = 1.f;

	void DisableControl();
	void EnableControl();

protected:
	//virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Game | Data", meta = (ClampMin="0", ClampMax="9"))
	int32 RespawnCount = 3;

	UFUNCTION()
	void TryRespawnPlayer(AActor* Killer);

	/** Alarming system begins */
public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnAlarmingModeChanged OnAlarmingModeChangedEvent;

protected:
	UFUNCTION(BlueprintCallable)   // TEST // DEBUG
	void EnableAlarmingSystem(const EAlarmingModeName& SwitchToModeName = EAlarmingModeName::NORMAL);
	UFUNCTION(BlueprintCallable)   // TEST // DEBUG
	void DisableAlarmingSystem();

	virtual void OnAlarmingModeChanged(const EAlarmingModeName& NewModeName, const EAlarmingModeName& LastModeName);

	EAlarmingModeName CurrentAlarmingModeName = EAlarmingModeName::NORMAL;
	TArray<class UAudioComponent*> CurrentAlarmModeSound_AudioComponents = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data | AlarmingSystem")
	TArray<FAlarmingModeData> AlarmingModes;          // 4 modes - normal, safe, gray-zone, and danger

	void Setup_AlarmLights();

	// Audio begin
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<USoundMix> GameSoundMix = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<USoundClass> MusicSoundClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<USoundClass> EffectsSoundClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<USoundClass> DialoguesSoundClass;
	// Audio end

private:
	TArray<AElectric_Siren_SpotLight*> SirensCache;
	TArray<AElectronicDoor*> DoorsCache;
	TArray<AAISpawnerBase*> AISpawnersCache;

	FAlarmingModeData GetAlarmingModeData_ByName(const EAlarmingModeName& ModeName);
	/** Alarming system ends */

public:
	//UFUNCTION(BlueprintCallable, Category = "Game | Rebirth")
	FORCEINLINE bool CanRespawn() const { return RespawnCount > 0 && BChar; }
	
	//UFUNCTION(BlueprintCallable, Category = "Game | Rebirth")
	void RespawnPlayer();
};

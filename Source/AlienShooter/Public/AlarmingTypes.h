#pragma once

#include "CoreMinimal.h"
#include "AlarmingTypes.generated.h"

UENUM(BlueprintType)
enum class EAlarmingModeName : uint8
{
	NORMAL = 0, SAFE = 1, GRAY_ZONE = 2, DANGER = 3, NONE = 99
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlarmingModeActivated, EAlarmingModeName, ModeName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlarmingModeDeactivated, EAlarmingModeName, ModeName);

USTRUCT(BlueprintType)
struct FAlarmingModeData   // 4 modes - normal, safe, gray-zone, and danger
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data)
	class AAlarmDirectionalLight_Electric* DirectionalLight;      // using ElectricalDirectionalLight class for now - change later if need to
	//ACustomDirectionalLight_DerivedFromElectricDirectionalLight* DirectionalLight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data)
	TArray<class USoundBase*> SirensSoundEffects;     // not applicable for normal mode

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data)
	EAlarmingModeName AlarmingModeName = EAlarmingModeName::NORMAL;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data)
	class UUserWidget* CursorWidget;

	UPROPERTY(BlueprintAssignable, Category=Data)
	FOnAlarmingModeActivated OnAlarmingModeActivated;
	
	UPROPERTY(BlueprintAssignable, Category=Data)
	FOnAlarmingModeDeactivated OnAlarmingModeDeactivated;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data)
	bool bForceDoorsOpen = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data)
	bool bDisableAISpawnStarters = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (ClampMin = "0", ClampMax = "100"))
	float AISpawnerTriggerChance = 0.f;
};

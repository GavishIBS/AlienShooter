// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Electricity/Actors/ElectricDirectionalLight.h"
#include "AlarmingTypes.h"
#include "AlarmDirectionalLight_Electric.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AAlarmDirectionalLight_Electric : public AElectricDirectionalLight
{
	GENERATED_BODY()
	
public:
	AAlarmDirectionalLight_Electric();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	EAlarmingModeName AlarmingModeName = EAlarmingModeName::NONE;

public:
	FORCEINLINE EAlarmingModeName GetLinkedAlarmingModeName() const { return AlarmingModeName; }
	
	FORCEINLINE void SetLightVisibility(const bool InValue);
};

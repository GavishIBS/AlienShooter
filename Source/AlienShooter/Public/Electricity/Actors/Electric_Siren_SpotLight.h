// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Electricity/Actors/ElectricSpotLight.h"
#include "AlarmingTypes.h"
#include "Electric_Siren_SpotLight.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AElectric_Siren_SpotLight : public AElectricSpotLight
{
	GENERATED_BODY()

public:
	AElectric_Siren_SpotLight();
	virtual void Tick(float DeltaTime) override;

	void TurnOnLightWithMode(const EAlarmingModeName& ModeName);

	UFUNCTION(BlueprintImplementableEvent)
	void SetLightProperties(const EAlarmingModeName& ModeName);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> SirenMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	FRotator SirenRotationSpeed = {12.f, 0.f, 0.f};
};

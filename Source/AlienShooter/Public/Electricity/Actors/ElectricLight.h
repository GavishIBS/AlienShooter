// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ElectronicActor.h"
#include "ElectricLight.generated.h"

class ULightComponent;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AElectricLight : public AElectronicActor
{
	GENERATED_BODY()
	
public:
	AElectricLight();
	virtual void Tick(float DeltaTime) override;

	void TurnOnLight();

protected:
	virtual void BeginPlay() override;
	virtual void OnElectricityTurnedOn() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<ULightComponent> Light;

	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bTurnOnAuto = true;

	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bInterpolateIntensity = false;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float IntensityInterpSpeed = 5.f;

	float MaxIntensity;
	bool bInterpingIntensity = false;
};

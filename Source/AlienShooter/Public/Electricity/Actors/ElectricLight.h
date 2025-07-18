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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	void TurnOnLight();

protected:
	virtual void BeginPlay() override;
	virtual void OnElectricityTurnedOn() override;

	void FlickerLight();

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
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bFlicker = false;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (ClampMin = "0"))
	float FlickerRate = 0.2f;  // TODO: Add flickering sound
	
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (ClampMin = "0"))
	float FlickerHoldTime = 0.1f;  // TODO: Add flickering sound

private:
	FTimerHandle FlickerTimerHandle;
	FTimerHandle FlickerHoldTimerHandle;
};

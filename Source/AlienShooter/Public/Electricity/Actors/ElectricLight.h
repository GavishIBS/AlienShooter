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

protected:
	virtual void BeginPlay() override;
	virtual void OnElectricityTurnedOn() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<ULightComponent> Light;
};

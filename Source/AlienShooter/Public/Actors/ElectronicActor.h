// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectronicActor.generated.h"

UCLASS()
class ALIENSHOOTER_API AElectronicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AElectronicActor();

protected:
	virtual void BeginPlay() override;
	virtual void OnElectricityTurnedOn();

private:
	bool bHaveElectricity = false;

public:
	FORCEINLINE bool HaveElectricity() const { return bHaveElectricity; }

	FORCEINLINE void SetHaveElectricity(const bool Value)
	{
		bHaveElectricity = Value;
		OnElectricityTurnedOn();
	}
};

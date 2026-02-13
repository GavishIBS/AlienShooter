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

protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bHaveElectricity = false;

public:
	FORCEINLINE bool HaveElectricity() const { return bHaveElectricity; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetHaveElectricity(const bool Value)
	{
		if (bHaveElectricity == Value) return;
		bHaveElectricity = Value;
		if (bHaveElectricity)
			OnElectricityTurnedOn();
	}
};

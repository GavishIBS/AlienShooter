// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	// Data
	FName PlayerName = "MyName";
	unsigned int AvailableCashAmount = 0;
	unsigned int NumGasCanisters = 0;

public:
	FORCEINLINE void AddCash(unsigned int InAmount) { AvailableCashAmount += InAmount; }
	FORCEINLINE void DeductCash(unsigned int InAmount) { AvailableCashAmount -= InAmount; }
	FORCEINLINE void AddGasCanister(unsigned int InValue) { NumGasCanisters += InValue; }



	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetPlayerName() const { return PlayerName; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetAvailableCash() { return (int)AvailableCashAmount; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetNumGasCanisters() { return (int)NumGasCanisters; }
};

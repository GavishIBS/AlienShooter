// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGasCanistersUpdated, int32, NumGasCanisters);

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnGasCanistersUpdated OnGasCanistersUpdated;

protected:
	// Data
	FName PlayerName = "MyName";
	unsigned int AvailableCashAmount = 0;
	unsigned int NumGasCanisters = 0;

public:
	FORCEINLINE void AddCash(unsigned int InAmount) { AvailableCashAmount += InAmount; }
	FORCEINLINE void DeductCash(unsigned int InAmount) { AvailableCashAmount -= InAmount; }
	FORCEINLINE void AddGasCanister(unsigned int InValue)
	{
		NumGasCanisters += InValue;
		OnGasCanistersUpdated.Broadcast(NumGasCanisters);
	}



	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetPlayerName() const { return PlayerName; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetAvailableCash() { return (int)AvailableCashAmount; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetNumGasCanisters() { return (int)NumGasCanisters; }


	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetNumGasCanisters(const int32 InValue)
	{
		NumGasCanisters = InValue;
		OnGasCanistersUpdated.Broadcast(NumGasCanisters);
	}
};

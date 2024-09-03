// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Electricity/Actors/ElectronicDoor.h"
#include "LoakableElectronicDoor.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ALoakableElectronicDoor : public AElectronicDoor
{
	GENERATED_BODY()
	
public:
	ALoakableElectronicDoor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void OnLockOpened();

private:
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bLocked = false;

public:
	FORCEINLINE bool IsLocked() const { return bLocked; }
	FORCEINLINE void SetLocked(const bool Value)
	{
		if (bLocked == Value) return;
		bLocked = Value;
		if (bLocked)
			OnLockOpened();
	}

	// DEBUG
	UFUNCTION(BlueprintCallable)
	void OpenLock()
	{
		SetLocked(false);
	}
};

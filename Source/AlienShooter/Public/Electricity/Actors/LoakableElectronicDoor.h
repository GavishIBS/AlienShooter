// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Electricity/Actors/ElectronicDoor.h"
#include "LoakableElectronicDoor.generated.h"

class AInventoryPickup;

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

	virtual bool TryUnlockDoor(AActor* Caller) override;

private:
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bLocked = false;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AInventoryPickup> DoorKey_Class;

public:
	FORCEINLINE virtual bool IsLocked() const override { return bLocked; }
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

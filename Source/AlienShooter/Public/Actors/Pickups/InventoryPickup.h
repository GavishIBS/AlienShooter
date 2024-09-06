// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/Pickup.h"
#include "Utilities.h"
#include "InventoryPickup.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AInventoryPickup : public APickup
{
	GENERATED_BODY()
	
public:
	AInventoryPickup();

protected:
	virtual void BeginPlay() override;
	virtual bool PostPickup(AActor* Caller) override;

	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bGenerateOverlapEvents = false;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	FItemData ItemData = FItemData();
};

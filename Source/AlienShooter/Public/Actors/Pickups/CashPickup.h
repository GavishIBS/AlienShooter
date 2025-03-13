// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/Pickup.h"
#include "CashPickup.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ACashPickup : public APickup
{
	GENERATED_BODY()

public:
	ACashPickup();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Properties")
	unsigned int Amount = 50;
};

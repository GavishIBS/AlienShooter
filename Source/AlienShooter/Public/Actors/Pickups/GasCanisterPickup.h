// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/Pickup.h"
#include "GasCanisterPickup.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AGasCanisterPickup : public APickup
{
	GENERATED_BODY()

public:
	AGasCanisterPickup();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/Pickup.h"
#include "AmmoPickup.generated.h"

class AWeaponBase;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AAmmoPickup : public APickup
{
	GENERATED_BODY()
	
public:
	AAmmoPickup();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<AWeaponBase> FitWeaponClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Properties")
	int AmmoValue = 30;
};

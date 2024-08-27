// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/Pickup.h"
#include "AttributePickup.generated.h"

UENUM()
enum class EAttribute
{
	NONE, HEALTH
};

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AAttributePickup : public APickup
{
	GENERATED_BODY()
	
public:
	AAttributePickup();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Properties")
	EAttribute AttributeSelection = EAttribute::NONE;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float AdditiveValue = 25.f;
};

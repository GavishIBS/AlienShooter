// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Actors/HittableActor.h"
#include "BreakableActor.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ABreakableActor : public AHittableActor
{
	GENERATED_BODY()
	
public:
	ABreakableActor();

	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void CreateField();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Geometry)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
};

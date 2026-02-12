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
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType = nullptr) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CreateField();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Geometry)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
};

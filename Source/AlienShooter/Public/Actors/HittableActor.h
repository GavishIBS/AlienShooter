// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "HittableActor.generated.h"

class URadialFalloff;
class UFieldSystem;

UCLASS()
class ALIENSHOOTER_API AHittableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	AHittableActor();

	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateField();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Geometry)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
};

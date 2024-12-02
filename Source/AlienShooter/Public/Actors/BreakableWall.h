// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ReplaceableWall.h"
#include "Interfaces/HitInterface.h"
#include "BreakableWall.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ABreakableWall : public AReplaceableWall, public IHitInterface
{
	GENERATED_BODY()
	
public:
	ABreakableWall();

	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitCallbacks();

	UFUNCTION()
	void OnTakeDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/InventoryPickup.h"
#include "DamageTypes.h"
#include "Interfaces/ExplodeInterface.h"
#include "Dynamite.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ADynamite : public AInventoryPickup, public IExplodeInterface
{
	GENERATED_BODY()
	
public:
	ADynamite();
	virtual void Explode(AActor* ThisObject, class UParticleSystem* InExplosionParticleSystem, const FDamageProperties& InDamageProperties,
		const FVector& Loc, const FRotator& Rot, const TArray<AActor*> InActorsToIgnore) override;
	void ExplodeWrapper();

	virtual void PlantDynamiteAtLocation(const FVector& Location, const FRotator& Rotation);

protected:
	virtual void BeginPlay() override;

	// Properties
	UPROPERTY(EditAnywhere, Category = "Properties")
	float ExplosionDelay = 7.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FDamageProperties DamageProperties = FDamageProperties();

	UPROPERTY(EditAnywhere, Category = "Properties")
	UParticleSystem* ExplosionParticleSystem = nullptr;

private:
	void StartExplosionTimer();

	FTimerHandle ExplosionTimerHandle;
};

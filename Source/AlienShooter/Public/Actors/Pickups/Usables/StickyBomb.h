// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/UsableActor.h"
#include "DamageTypes.h"
#include "StickyBomb.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AStickyBomb : public AUsableActor
{
	GENERATED_BODY()
	
public:
	AStickyBomb();
	UFUNCTION(BlueprintCallable)
	virtual bool UseItem(ACharacter* User) override;

protected:
	virtual void BeginPlay() override;
	virtual bool CanUse(ACharacter* User, FHitResult& HitResult) override;

	void Explode();

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

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DamageTypes.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "GasCylinder.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AGasCylinder : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:
	AGasCylinder();

	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void SpawnEffects();
	void ApplyDamage();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> GasCylinderMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> ExplosionOriginPoint;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TObjectPtr<UParticleSystem> ExplosionParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FDamageProperties DamageProperties = FDamageProperties();

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DestroyDelay = 5.f;

private:
	FTimerHandle DestroyTimerHandle;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Actors/HittableActor.h"
#include "Explosive.generated.h"

class UNiagaraSystem;
class USphereComponent;
class AGameCharacter;

USTRUCT(BlueprintType)
struct FDamageProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Properties")
	float BaseDamage = 150.f;
	
	UPROPERTY(EditAnywhere, Category="Properties")
	float MinimumDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamageInnerRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamageOuterRadius = 300.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamageFalloff = 2.f;
};

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AExplosive : public AHittableActor
{
	GENERATED_BODY()
	
public:
	AExplosive();

	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ExplosiveMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> ExplosionOriginPoint;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSoftObjectPtr<UNiagaraSystem> ExplosionParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<AActor> FilterClass;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FDamageProperties DamageProperties = FDamageProperties();
};

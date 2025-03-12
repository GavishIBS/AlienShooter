// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Actors/HittableActor.h"
#include "DamageTypes.h"
#include "../Interfaces/ExplodeInterface.h"
#include "Explosive.generated.h"

class UNiagaraSystem;
class USphereComponent;
class AGameCharacter;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AExplosive : public AHittableActor, public IExplodeInterface
{
	GENERATED_BODY()
	
public:
	AExplosive();
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass) override;
	virtual void Explode(AActor* ThisObject, class UParticleSystem* InExplosionParticleSystem, const FDamageProperties& InDamageProperties,
		const FVector& Loc, const FRotator& Rot, const TArray<AActor*> InActorsToIgnore) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitCallbacks();

	//UFUNCTION()
	//void OnTakeDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ExplosiveMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> ExplosionOriginPoint;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSoftObjectPtr<UNiagaraSystem> ExplosionParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FDamageProperties DamageProperties = FDamageProperties();
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	float ExplosionDelay = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float CameraShakeRange = 700.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float ForceAffectingSphereRadius = 200.f;

	//UPROPERTY(EditAnywhere, Category = "Properties")
	//float ForceMagnitude = 2500.f;

private:
	FTimerHandle ExplosionTimerHandle;

	void ExplodeWrapper();
	void OnExplosion();




public:
	// TEMP - Don't use cascade particle systems
	UPROPERTY(EditAnywhere, Category = "Properties")
	UParticleSystem* ExplosionParticleSystem_C = nullptr;
};

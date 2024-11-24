// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"
#include "Weapons/WeaponTypes.h"
#include "Projectile.generated.h"

class AProjectileWeapon;
class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class ALIENSHOOTER_API AProjectile : public APooledActor
{
	GENERATED_BODY()
	
public:
	friend AProjectileWeapon;
	AProjectile();
	virtual void Tick(float DeltaTime) override;

	//virtual void Init(const float& InBulletHealth, const float& InDamage, const float& InDamage_RandomizationFactor);
	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	// Ownership
	AActor* WeaponOwner = nullptr;
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	// Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	FProjectileProperties ProjectileProperties;

protected:
	FVector TargetLocation;
	
	UFUNCTION()
	void DestroyWrapper();

	virtual void UpdateProjectile();

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float ProjectileUpdateRate = 0.1f;

private:
	FTimerHandle DestroyTimerHandle;
	FTimerHandle ProjectileTimerHandle;

public:
	virtual void SetInUse(const bool InValue) override;

	FORCEINLINE void SetTargetLocation(const FVector& TargetLoc);
};

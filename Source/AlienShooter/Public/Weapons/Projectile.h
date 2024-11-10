// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponTypes.h"
#include "Projectile.generated.h"

class AProjectileWeapon;
class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class ALIENSHOOTER_API AProjectile : public AActor
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
	
private:
	UFUNCTION()
	void DestroyWrapper();

	FTimerHandle DestroyTimerHandle;

public:
	FORCEINLINE void SetTargetLocation(const FVector& TargetLoc);
};

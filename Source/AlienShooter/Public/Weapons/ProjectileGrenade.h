// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectile.h"
#include "DamageTypes.h"
#include "ProjectileGrenade.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AProjectileGrenade : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectileGrenade();

protected:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> GrenadeMesh;

	// Properties
	UPROPERTY(EditAnywhere, Category = "Properties")
	FDamageProperties DamageProperties = FDamageProperties();

	UPROPERTY(EditAnywhere, Category = "Properties")
	UParticleSystem* ExplosionParticleSystem = nullptr;
};

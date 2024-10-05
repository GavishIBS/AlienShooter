// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectile.h"
#include "Interfaces/BulletInterface.h"
#include "ProjectileBullet.generated.h"

class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AProjectileBullet : public AProjectile, public IBulletInterface
{
	GENERATED_BODY()

public:
	AProjectileBullet();
	virtual void OnBulletHit(AActor* HitActor, const FVector& ImpactPoint) override;
	//virtual void Init(const float& InBulletHealth, const float& InDamage, const float& InDamage_RandomizationFactor) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UStaticMeshComponent> BulletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> Tracer_FX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FProjectileBulletData ProjectileBulletData = FProjectileBulletData();
};

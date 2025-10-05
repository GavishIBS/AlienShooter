// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAttackProjectile.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class ALIENSHOOTER_API AEnemyAttackProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyAttackProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void BindCallbacks();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	void OnAttackProjectileHit(AActor* HitActor, UPrimitiveComponent* HitComponent);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraSystem> HitFX;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UMaterialInstance> HitDecal;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	FVector HitDecalSize = FVector(200, 200, 100);
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	float HitDecalLifetime = 5.f;
};

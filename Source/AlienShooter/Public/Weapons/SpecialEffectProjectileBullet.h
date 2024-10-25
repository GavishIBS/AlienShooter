// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ProjectileBullet.h"
#include "../SpecialWeaponTypes.h"
#include "SpecialEffectProjectileBullet.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ASpecialEffectProjectileBullet : public AProjectileBullet
{
	GENERATED_BODY()

public:
	ASpecialEffectProjectileBullet();

protected:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FSpecialEffectData SpecialEffectData = FSpecialEffectData();
};

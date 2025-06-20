// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "Interfaces/BulletInterface.h"
#include "TracerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ATracerWeapon : public AWeaponBase, public IBulletInterface
{
	GENERATED_BODY()
	
public:
	ATracerWeapon();
	virtual void OnBulletHit(AActor* HitActor, const FVector& ImpactPoint, UPhysicalMaterial* PhysMat = nullptr) override;

protected:
	virtual void BeginPlay() override;
	virtual void Fire() override;

	UPROPERTY(EditAnywhere, Category = "Weapon Data")
	FTracerWeaponData TracerWeaponData = FTracerWeaponData();
};

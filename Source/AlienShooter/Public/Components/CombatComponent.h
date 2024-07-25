// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AWeaponBase;
class AShooterCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend AShooterCharacter;
	UCombatComponent();

protected:
	virtual void BeginPlay() override;

	virtual void Init_Component();

	void AddWeapon(AWeaponBase* InWeapon);
	bool SelectWeapon(const int Index);
	
	UPROPERTY(VisibleAnywhere, Category=Combat)
	TArray<TSubclassOf<AWeaponBase>> Weapons = TArray<TSubclassOf<AWeaponBase>>();

	UPROPERTY(VisibleAnywhere, Category=Combat)
	AWeaponBase* WeaponInHand = nullptr;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category=Ownership)
	AShooterCharacter* OwnerAsShooterCharacter = nullptr;
};

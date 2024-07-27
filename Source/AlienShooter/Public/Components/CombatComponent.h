// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeaponBase;
class AShooterCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend AShooterCharacter;
	UCombatComponent();

	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category=Delegates)
	FOnWeaponUpdated OnWeaponUpdated;

protected:
	virtual void BeginPlay() override;

	virtual void Init_Component();

	bool CanTakeWeapon(const AWeaponBase* InWeapon) const;
	
	bool AddWeapon(AWeaponBase* InWeapon);
	bool SelectWeapon(const int Index);

	void FireButtonPressed();

	UFUNCTION()
	void OnWeaponFiredCallback();

	void OnWeaponSelected();
	void OnWeaponDestroyBegin(AWeaponBase* InWeapon);
	
	void UpdateAmmoForCurrentWeapon();
	
	UPROPERTY(VisibleAnywhere, Category=Combat)
	TMap<TSubclassOf<AWeaponBase>, int> Weapons;

	UPROPERTY(VisibleAnywhere, Category=Combat)
	AWeaponBase* WeaponInHand = nullptr;

	UPROPERTY(VisibleAnywhere, Category=Combat)
	int8 WeaponIndex = -1;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category=Ownership)
	AShooterCharacter* OwnerAsShooterCharacter = nullptr;
};

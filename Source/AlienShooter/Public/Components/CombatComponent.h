// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeaponBase;
class AShooterCharacter;
class AAmmoPickup;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewWeaponAdded, const AWeaponBase*, InWeapon);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend AShooterCharacter;
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category=Delegates)
	FOnWeaponUpdated OnWeaponUpdated;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category=Delegates)
	FOnNewWeaponAdded OnNewWeaponAdded;
	
protected:
	virtual void BeginPlay() override;

	virtual void Init_Component();

	bool CanTakeWeapon(const AWeaponBase* InWeapon) const;
	
	bool AddWeapon(AWeaponBase* InWeapon);
	bool SelectWeapon(const int Index);
	bool UnselectWeapon();

	void FireButtonPressed();

	UFUNCTION()
	void OnWeaponFiredCallback();

	void OnWeaponSelected();
	void OnWeaponDestroyBegin(AWeaponBase* InWeapon);
	
	void UpdateAmmoForCurrentWeapon();

	bool CanTakeAmmo(const TSubclassOf<AWeaponBase>& FitWeaponClass);

	void DestroyCurrentWeapon(); // Destorys weapon currently in hand

public:
	bool TakeAmmo(const TSubclassOf<AWeaponBase>& FitWeaponClass, const int& AmmoValue);
	
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

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ControllableCharacter.h"
#include "Components/CombatComponent.h"
#include "Weapons/WeaponBase.h"
#include "ShooterCharacter.generated.h"

class AWeaponBase;
class UCombatComponent;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AShooterCharacter : public AControllableCharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void PickupWeapon(AWeaponBase* InWeapon) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UCombatComponent> CombatComponent = nullptr;
	
	/* Input callbacks begin */
	void LeftMouseButtonPressed();
	/* Input callbacks end */
	
	/* Input begin */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> LeftMouseButtonPressedAction;
	/* Input end */

public:
	FORCEINLINE bool IsEquipped() const {return CombatComponent ? CombatComponent->WeaponInHand ? true : false : false;}
	FORCEINLINE EWeaponName GetWeaponInHand_Name() const {return CombatComponent ? CombatComponent->WeaponInHand ?
			CombatComponent->WeaponInHand->GetWeaponProperties().WeaponName : EWeaponName::EWN_None : EWeaponName::EWN_None;}
};

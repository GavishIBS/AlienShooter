// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterTypes.h"
#include "Characters/ControllableCharacter.h"
#include "Components/CombatComponent.h"
#include "Weapons/WeaponBase.h"
#include "../CustomAudioTypes.h"
#include "ShooterCharacter.generated.h"

class AWeaponBase;
class UCombatComponent;
class USpecialAbilityComponent;
class UNiagaraComponent;

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

	/* Interfaces begin */
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass) override;
	/* Interfaces end */
	
	void PickupWeapon(AWeaponBase* InWeapon) const;

	void TryUseInventoryItemAtIndex(const int Index);

protected:
	virtual void BeginPlay() override;

	void InitCallbacks();

	UFUNCTION()
	virtual void OnRadioPhoneAudioTriggered(const EAudioType AudioType, const FString AudioInfo);

	UFUNCTION()
	void OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	virtual void OnDeathCallback(AActor* Killer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effects | Components")
	TObjectPtr<UNiagaraComponent> SpecialAbilityNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UCombatComponent> CombatComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<USpecialAbilityComponent> SpecialAbilityComponent;

	/* Input callbacks begin */
	void LeftMouseButtonPressed();
	void LeftMouseButtonReleased();
	void MouseWheelUp();
	void MouseWheelDown();
	void LeftAltPressed();
	void LeftAltReleased();
	void AbilityKeyPressed();
	void InventorySlotPressed_1();
	void InventorySlotPressed_2();
	void InventorySlotPressed_3();
	void InventorySlotPressed_4();
	/* Input callbacks end */
	
	/* Input begin */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputAction> LeftMouseButtonAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputAction> MouseWheelUpAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputAction> MouseWheelDownAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LeftAltAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> SpecialAbilityAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InventorySlot_1_Action = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InventorySlot_2_Action = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InventorySlot_3_Action = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InventorySlot_4_Action = nullptr;
	/* Input end */

	FShooterProperties ShooterProperties = FShooterProperties();

protected:
	UFUNCTION(BlueprintCallable)
	void EndAttack();

	UFUNCTION(BlueprintCallable)
	void EndReload();

	UFUNCTION(BlueprintCallable)
	void OnInventoryItemSuccessfullyUsed();

	UFUNCTION()
	void OnWeaponInHandUpdated();

	UFUNCTION()
	void OnWeaponFired();

	UFUNCTION()
	void OnWeaponReload();

private:
	// Inventory UI
	void OpenInventory_UI();
	void CloseInventory_UI();

public:
	bool CanFire();

	FORCEINLINE TObjectPtr<UCombatComponent> GetCombatComponent() const {return CombatComponent;}
	
	FORCEINLINE bool IsEquipped() const {return CombatComponent ? CombatComponent->WeaponInHand ? true : false : false;}
	FORCEINLINE TObjectPtr<AWeaponBase> GetWeaponInHand() const {return CombatComponent ? CombatComponent->WeaponInHand : nullptr;}
	FORCEINLINE EWeaponName GetWeaponInHand_Name() const {return CombatComponent ? CombatComponent->WeaponInHand ?
			CombatComponent->WeaponInHand->GetWeaponProperties()->WeaponName : EWeaponName::EWN_None : EWeaponName::EWN_None;}
	FORCEINLINE FShooterProperties* GetShooterProperties() {return &ShooterProperties;}
	FORCEINLINE int GetNumberOfWeapon() const {return CombatComponent ? CombatComponent->Weapons.Num() : 0;}

	FORCEINLINE void DeactivateSpecialAbilityNiagaraEffect();
	FORCEINLINE void ActivateSpecialAbilityNiagaraEffect(UNiagaraSystem* InSystem);
};

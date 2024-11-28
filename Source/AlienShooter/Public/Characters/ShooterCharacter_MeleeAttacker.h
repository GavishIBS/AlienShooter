// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ShooterCharacter.h"
#include "ShooterCharacter_MeleeAttacker.generated.h"

class AMeleeWeaponBase;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AShooterCharacter_MeleeAttacker : public AShooterCharacter
{
	GENERATED_BODY()
	
public:
	AShooterCharacter_MeleeAttacker();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void MeleeAttackKeyDown();
	void MeleeAttackKeyUp();

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* MeleeAttackAction;

protected:
	bool CanAttack();

	UFUNCTION(BlueprintCallable)
	void EndMeleeAttack();

	UFUNCTION(BlueprintCallable)
	void StartMeleeAttackTrace();

	UFUNCTION(BlueprintCallable)
	void StopMeleeAttackTrace();

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	TSubclassOf<AMeleeWeaponBase> MeleeWeaponClass;
	
	AMeleeWeaponBase* MeleeWeapon;
};

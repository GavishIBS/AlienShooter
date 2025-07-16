// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapons/WeaponTypes.h"
#include "ShooterAnimInstance.generated.h"

class AShooterCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UShooterAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	void CalculateIK(float DeltaSeconds);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ownership")
	TObjectPtr<AShooterCharacter> ShooterCharacter;
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	FName RightHandSocketName = FName("RightHandSocket");
	
	UPROPERTY(EditDefaultsOnly, Category=Properties)
	FName RightHandBoneName = FName("hand_r");
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Locomotion)
	FVector Velocity = FVector();
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Locomotion)
	float SpeedXY = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Locomotion)
	bool bFalling = false;

	FRotator DeltaRotation = FRotator();
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Locomotion)
	float YawOffsetFromVelocity = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Locomotion)
	float TurnInPlace_Yaw = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Combat)
	bool bEquipped = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Combat)
	TObjectPtr<AWeaponBase> WeaponInHand = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Combat)
	EWeaponName WeaponInHand_Name = EWeaponName::EWN_None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Combat)
	bool bAiming = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=IK)
	bool bEnableFabrik = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=IK)
	FTransform LeftHandTransform = FTransform();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=IK)
	bool bEnableRightHandIK = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=IK)
	FRotator RightHandRotation = FRotator();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=IK)
	FRotator RightHandRotation_LastFrame = FRotator();


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=IK)
	bool bRadioPhoneAudioInProgress = false;
};

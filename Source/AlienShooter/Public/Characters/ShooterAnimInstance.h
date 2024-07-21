// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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
	TObjectPtr<AShooterCharacter> ShooterCharacter;
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Locomotion)
	FVector Velocity = FVector();
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Locomotion)
	float SpeedXY = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Locomotion)
	bool bFalling = false;

	FRotator DeltaRotation;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Locomotion)
	float YawOffsetFromVelocity = 0.f;
};

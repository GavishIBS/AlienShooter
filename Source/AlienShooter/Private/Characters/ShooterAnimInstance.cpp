// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Characters/ShooterAnimInstance.h"
#include "Characters/ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UShooterAnimInstance::UShooterAnimInstance()
{
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	if (ShooterCharacter)
		MovementComponent = ShooterCharacter->GetCharacterMovement();
}

void UShooterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!ShooterCharacter || !MovementComponent)
		return;

	{
		// Locomotion
		Velocity = ShooterCharacter->GetVelocity();

		const FVector VelocityXY = FVector(Velocity.X, Velocity.Y, 0.f);
		SpeedXY = VelocityXY.Size();

		bFalling = MovementComponent->IsFalling();

		if (!FMath::IsNearlyEqual(SpeedXY, 0.f, 1.f))
		{
			FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
			AimRotation.Yaw = ShooterCharacter->GetMesh()->GetComponentRotation().Yaw - 90.f;
			const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());
			const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
			DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 10.f);
			YawOffsetFromVelocity = DeltaRotation.Yaw;
		} else YawOffsetFromVelocity = 0.f;
	}

	{
		// Combat
		bEquipped = ShooterCharacter->IsEquipped();
		WeaponInHand = ShooterCharacter->GetWeaponInHand();
		WeaponInHand_Name = ShooterCharacter->GetWeaponInHand_Name();
	}

	{
		// IK
		bEnableRightHandIK = ShooterCharacter->GetShooterProperties()->bEnableIK;
		if (bEquipped && WeaponInHand && bEnableRightHandIK)
		{
			const FVector RightHandLocation = ShooterCharacter->GetMesh()->GetSocketLocation(RightHandSocketName);
			RightHandRotation = ShooterCharacter->GetMesh()->GetSocketRotation(RightHandSocketName);
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandLocation,
				RightHandLocation + (RightHandLocation - ShooterCharacter->GetCursorTraceHitLocation()));
			RightHandRotation.Yaw = LookAtRotation.Yaw;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f"), RightHandRotation.Yaw));
		}
	}
}

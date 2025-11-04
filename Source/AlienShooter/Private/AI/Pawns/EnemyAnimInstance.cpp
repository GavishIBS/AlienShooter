// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/Pawns/EnemyAnimInstance.h"
#include "AI/Pawns/BaseAICharacter.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerAICharacter = Cast<ABaseAICharacter>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerAICharacter)
	{
		const FVector Velocity = OwnerAICharacter->GetVelocity();
		const FVector VelocityXY = FVector(Velocity.X, Velocity.Y, 0.f);
		GroundSpeed = VelocityXY.Size();
	}
}

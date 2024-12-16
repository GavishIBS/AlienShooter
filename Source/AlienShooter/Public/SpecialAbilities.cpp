// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#include "SpecialAbilities.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

bool Dash(ACharacter* InCharacter) {
	if (!InCharacter) return false;

	/*
	FRotator ForwardDirectionRot = InCharacter->GetMesh()->GetComponentRotation();
	ForwardDirectionRot.Yaw += 90.f;
	FVector LaunchDirection = UKismetMathLibrary::GetForwardVector(ForwardDirectionRot);
	*/
	FVector LaunchDirection = InCharacter->GetVelocity().GetSafeNormal();
	InCharacter->LaunchCharacter(LaunchDirection * 3250.f, false, false);
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString("I am dashing"));
	return true;
}

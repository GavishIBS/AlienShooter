// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#include "SpecialAbilities.h"
#include "GameFramework/Character.h"

bool Dash(ACharacter* InCharacter) {
	if (!InCharacter) return false;

	//InCharacter->LaunchCharacter(FVector(1000.f, 600.f, 250.f), false, false);
	InCharacter->LaunchCharacter(InCharacter->GetMesh()->GetForwardVector() * 3250.f, false, false);
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString("I am dashing"));
	return true;
}

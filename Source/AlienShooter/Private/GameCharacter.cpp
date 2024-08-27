// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "GameCharacter.h"

AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add(FName("Character"));
}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

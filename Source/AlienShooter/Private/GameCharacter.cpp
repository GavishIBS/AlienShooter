// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "GameCharacter.h"

AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

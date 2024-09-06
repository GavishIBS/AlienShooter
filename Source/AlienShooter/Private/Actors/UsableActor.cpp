// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/UsableActor.h"

AUsableActor::AUsableActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUsableActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AUsableActor::CanUse(ACharacter* User)
{
	return false;
}

void AUsableActor::UseItem(ACharacter* User)
{
}

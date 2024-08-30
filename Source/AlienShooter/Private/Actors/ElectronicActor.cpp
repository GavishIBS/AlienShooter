// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/ElectronicActor.h"

AElectronicActor::AElectronicActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AElectronicActor::BeginPlay()
{
	Super::BeginPlay();
}

void AElectronicActor::OnElectricityTurnedOn()
{
}

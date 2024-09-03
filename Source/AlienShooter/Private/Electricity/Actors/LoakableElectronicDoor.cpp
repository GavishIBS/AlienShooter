// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/LoakableElectronicDoor.h"

ALoakableElectronicDoor::ALoakableElectronicDoor()
{
}

void ALoakableElectronicDoor::BeginPlay()
{
	Super::BeginPlay();
}

void ALoakableElectronicDoor::Tick(float DeltaTime)
{
	if (bLocked) return;
	Super::Tick(DeltaTime);
}

void ALoakableElectronicDoor::OnLockOpened()
{
}

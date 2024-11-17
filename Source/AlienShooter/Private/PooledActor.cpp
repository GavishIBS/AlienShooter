// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "PooledActor.h"

APooledActor::APooledActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APooledActor::BeginPlay()
{
	Super::BeginPlay();
}

void APooledActor::SetInUse(const bool InValue)
{
	bInUse = InValue;
	if (bInUse) {
		SetActorEnableCollision(true);
		//SetActorHiddenInGame(false);
		SetActorTickEnabled(true);
	}
	else {
		SetActorEnableCollision(false);
		//SetActorHiddenInGame(true);
		SetActorTickEnabled(false);
	}
}

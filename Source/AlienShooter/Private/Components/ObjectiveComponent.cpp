// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/ObjectiveComponent.h"

UObjectiveComponent::UObjectiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectiveComponent::BeginPlay()
{
	Super::BeginPlay();
}

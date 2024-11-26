// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/DialogueComponent.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

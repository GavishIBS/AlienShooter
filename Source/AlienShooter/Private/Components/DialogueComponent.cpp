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

bool UDialogueComponent::IsConvoEnd(const int InIndex)
{
	if (InIndex >= ConversationData.Num()) return true;
	return false;
}

FDialogueData UDialogueComponent::GetNextDialogueData(const int InIndex)
{
	if (InIndex < ConversationData.Num())
		return ConversationData[InIndex];
	return FDialogueData();
}

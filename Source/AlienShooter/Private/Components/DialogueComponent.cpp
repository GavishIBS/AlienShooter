// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/DialogueComponent.h"
#include "UI/Widgets/TDS_DialogueUW.h"

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

void UDialogueComponent::StartConversation(UTDS_DialogueUW* InDialogueScreenUW)
{
	DialogueScreenUW = InDialogueScreenUW;
	DialogueScreenUW->OnNextButtonDown.AddDynamic(this, &UDialogueComponent::StartNextDialogue);
	StartNextDialogue();   // Call to trigger first dialogue
}

void UDialogueComponent::StartNextDialogue()
{
	if (IsConvoEnd(++CurrentDialogueIndex) && DialogueScreenUW) {
		DialogueScreenUW->OnNextButtonDown.RemoveDynamic(this, &UDialogueComponent::StartNextDialogue);
		DialogueScreenUW->RemoveFromParent();
		DialogueScreenUW = nullptr;
		CurrentDialogueIndex = -1;
		return;
	}
	const FDialogueData NextDialogueData = GetNextDialogueData(CurrentDialogueIndex);
	DialogueScreenUW->SetTargetTextAndDialogueAudio(NextDialogueData.Dialogue, NextDialogueData.DialogueAudio);
}

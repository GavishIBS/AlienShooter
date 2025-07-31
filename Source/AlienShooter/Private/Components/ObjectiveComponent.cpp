// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/ObjectiveComponent.h"
#include "UI/Widgets/TDS_DialogueLogsUserWidget.h"

UObjectiveComponent::UObjectiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectiveComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UObjectiveComponent::RemoveDialogueLogsMenu_UW()
{
	DialogueLogsMenu_UW_Ref->RemoveFromParent();
	DialogueLogsMenu_UW_Ref = nullptr;
}

void UObjectiveComponent::SetDialogueLogs_Wrapper()
{
	if (DialogueLogsMenu_UW_Ref) {
		DialogueLogsMenu_UW_Ref->SetDialogueLogs(RadioPhoneDialogueLogs);
	}
}

void UObjectiveComponent::AddRadioPhoneDialogueLog(const FDialogueLog&& NewLog)
{
	RadioPhoneDialogueLogs.Add(NewLog);
}

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

bool UObjectiveComponent::AddObjective(const FRemindableObjective& Objective)
{
	if (Objective.ObjectiveData.Key == -1) return false;  // "Key" default value is -1, this statement makes sure that "Key" value was set

	DisplayObjectives.Add((FObjective)Objective);
	if (Objective.RemindingAudio != nullptr) {
		RemindObjectives.Add((FReminderInfo)Objective);
	}

	return true;
}

bool UObjectiveComponent::CompleteObjective(const FObjective& Objective)
{
	bool ReturnValue = false;
	int Index = 0;
	for (auto& Obj : DisplayObjectives)
	{
		if (Objective.Key == Obj.Key) {
			DisplayObjectives.RemoveAt(Index);
			ReturnValue = true;
			break;
		}
		Index++;
	}

	Index = 0;
	for (auto& Obj : RemindObjectives)
	{
		if (Objective.Key == Obj.Key) {
			RemindObjectives.RemoveAt(Index);
			break;
		}
		Index++;
	}

	return ReturnValue;
}

std::tuple<bool, FReminderInfo> UObjectiveComponent::TriggerReminder()
{
	if (RemindObjectives.Num() <= 0) return std::make_tuple(false, FReminderInfo());
	
	return std::make_tuple(true, RemindObjectives[0]);
	// TODO: Implementation - need a ref to radio phone component to trigger audio, but accessing it here will create a circular dependency
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_DialogueLogSlot_UserWidget.h"

UTDS_DialogueLogSlot_UserWidget::UTDS_DialogueLogSlot_UserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_DialogueLogSlot_UserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDS_DialogueLogSlot_UserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindCallbacks();
}

void UTDS_DialogueLogSlot_UserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_DialogueLogSlot_UserWidget::BindCallbacks()
{
}

void UTDS_DialogueLogSlot_UserWidget::SetDialogueLog_Values(const FName& InSpeakerName, const FString& InDialogueLogText)
{
	DialogueSpeakerName = InSpeakerName;
	DialogueLogText = InDialogueLogText;
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_DialogueUW.h"
#include "Sound/SoundWave.h"
#include "GameFramework/Pawn.h"

UTDS_DialogueUW::UTDS_DialogueUW(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_DialogueUW::InitComponent()
{
}

void UTDS_DialogueUW::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDS_DialogueUW::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTDS_DialogueUW::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_DialogueUW::StartWritingDialogue()
{
	if (!AudioWave) {
		TextOnScreen = TargetText;
		return;
	}

	float AudioLength = AudioWave->Duration;
	float NumChars = (float)TargetText.Len();
	float CharPrintRate = NumChars / AudioLength;

	// Timer to increment characters one by one
	if (DialogueInitiator) {
		DialogueInitiator->GetWorldTimerManager()
			.SetTimer(PrintingTimerHandle, this, &UTDS_DialogueUW::IncrementCharacter, CharPrintRate, true, 0.f);
	}
}

void UTDS_DialogueUW::IncrementCharacter()
{
	int RequiredIndex = TextOnScreen.Len();
	if (RequiredIndex >= TargetText.Len()) {
		if (DialogueInitiator) {
			DialogueInitiator->GetWorldTimerManager().ClearTimer(PrintingTimerHandle);
		}
		OnDialogueCompleted.Broadcast();
		return;
	}
	TextOnScreen += TargetText[RequiredIndex];
}

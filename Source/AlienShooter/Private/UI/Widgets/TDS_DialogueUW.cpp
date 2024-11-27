// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_DialogueUW.h"
#include "Sound/SoundWave.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

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
	if (NextButton)
		NextButton->OnClicked.AddDynamic(this, &UTDS_DialogueUW::OnNextButtonClicked);
}

void UTDS_DialogueUW::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_DialogueUW::OnNextButtonClicked()
{
	TextOnScreen = FString();
	OnNextButtonDown.Broadcast();
}

void UTDS_DialogueUW::StartWritingDialogue()
{
	if (!AudioWave) {
		TextOnScreen = TargetText;
		return;
	}

	// Disable next button untill dialogue is complete
	NextButton->SetIsEnabled(false);

	float AudioLength = AudioWave->Duration;
	float NumChars = TargetText.Len();
	float CharPrintRate = AudioLength / NumChars;

	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, FString::Printf(TEXT("AudioLength: %f\nNumChars: %f\nCharPrintRate: %f"),
		AudioLength, NumChars, CharPrintRate));

	// TODO: Playing audio should be handled from DialogueComponent, and this UW should not have access to dialogue audio asset
	if (DialogueInitiator)
		UGameplayStatics::PlaySoundAtLocation(DialogueInitiator, AudioWave, DialogueInitiator->GetActorLocation());

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
		NextButton->SetIsEnabled(true);
		//TextOnScreen = FString();
		return;
	}
	TextOnScreen += TargetText[RequiredIndex];
}

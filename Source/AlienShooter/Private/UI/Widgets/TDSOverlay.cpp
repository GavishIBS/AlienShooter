// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDSOverlay.h"
#include "UI/Widgets/TDSWeaponBoxList.h"
#include "UI/Widgets/TDSWeaponInHandDataBox.h"
#include "Components/ProgressBar.h"


#include "Characters/ShooterCharacter.h"
#include "Components/AttributeComponent.h"


UTDSOverlay::UTDSOverlay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDSOverlay::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (WeaponInHandDataBox)
		WeaponInHandDataBox->SetDesiredSizeInViewport(WeaponInHandDataBox->GetRootSize());
}

void UTDSOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	InitComponents();
	if (OwnerAsShooterCharacter) {
		OwnerAsShooterCharacter->GetAttributeComponent()->OnHealthUpdated.AddDynamic(this, &UTDSOverlay::UpdateHealthBar);
		OwnerAsShooterCharacter->GetAttributeComponent()->OnMaxHealthUpdated.AddDynamic(this, &UTDSOverlay::UpdateHealthBar);
	}
}

void UTDSOverlay::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDSOverlay::InitComponents()
{
	if (WeaponInHandDataBox)
	{
		WeaponInHandDataBox->SetOwnerCharacter(OwnerAsShooterCharacter);
		WeaponInHandDataBox->InitComponent();
	}
	/*
	if (WeaponBoxList)
	{
		WeaponBoxList->SetOwnerCharacter(OwnerAsShooterCharacter);
		WeaponBoxList->InitComponent();
	}*/
	if (HealthBar)
	{
		HealthBar->SetPercent(1);
	}
}

void UTDSOverlay::UpdateHealthBar(float Health, float MaxHealth)
{
	if (MaxHealth == 0) return;
	if (HealthBar) {
		HealthBar->SetPercent(Health / MaxHealth);
	}
}

void UTDSOverlay::SetEnvironmentDialogue_Subtitles_Text(const FText InText, const float& DialogueAudioDuration)
{
	//EnvironmentDialogue_Subtitles_Text = InText;
	
	Target_Subtitles_Text = InText;
	float TypingSpeed = DialogueAudioDuration / InText.ToString().Len();
	GetWorld()->GetTimerManager().
		SetTimer(TypeEnvironmentDialogue_Subtitles_Text_TimerHandle, this, &UTDSOverlay::IncrementCharacter_To_Subtitles_Text, TypingSpeed, true, 0.f);
	
	GetWorld()->GetTimerManager().SetTimer(EnvironmentDialogueText_Subtitles_TimerHandle, this, &UTDSOverlay::OnEnvironmentDialogueCompleted,
		DialogueAudioDuration + Environment_Subtitles_Text_ClearDelay, false, DialogueAudioDuration + Environment_Subtitles_Text_ClearDelay);
}

void UTDSOverlay::IncrementCharacter_To_Subtitles_Text()
{
	if (Target_Subtitles_Text.ToString().Len() <= EnvironmentDialogue_Subtitles_Text.ToString().Len()) {
		GetWorld()->GetTimerManager().ClearTimer(TypeEnvironmentDialogue_Subtitles_Text_TimerHandle);
		return;
	}

	FString NewText_AsString = EnvironmentDialogue_Subtitles_Text.ToString();
	NewText_AsString += Target_Subtitles_Text.ToString()[EnvironmentDialogue_Subtitles_Text.ToString().Len()];
	EnvironmentDialogue_Subtitles_Text = FText::FromString(NewText_AsString);
}

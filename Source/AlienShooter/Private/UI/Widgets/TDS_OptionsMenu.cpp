// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_OptionsMenu.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Game/Subsystems/OptionsGameInstanceSubsystem.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"

UTDS_OptionsMenu::UTDS_OptionsMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_OptionsMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDS_OptionsMenu::NativeConstruct()
{
	Super::NativeConstruct();

	OptionsGIS = GetGameInstance()->GetSubsystem<UOptionsGameInstanceSubsystem>();

	BindCallbacks();
	
	DisplayDefaultSettings();
}

void UTDS_OptionsMenu::NativeDestruct()
{
	Super::NativeDestruct();
	OnSaveSettingsButtonPressed();
}

void UTDS_OptionsMenu::BindCallbacks()
{
	if (BackButton) {
		BackButton->OnClicked.AddDynamic(this, &UTDS_OptionsMenu::OnBackButtonPressed);
	}
	if (SaveSettingsButton) {
		SaveSettingsButton->OnClicked.AddDynamic(this, &UTDS_OptionsMenu::OnSaveSettingsButtonPressed);
	}
	if (EnableCameraBoomOffset_Checkbox && EnableCameraBoomRotation_Checkbox) {
		EnableCameraBoomOffset_Checkbox->OnCheckStateChanged.AddDynamic(this, &UTDS_OptionsMenu::OnCameraBoomOffset_CheckboxChanged);
		EnableCameraBoomRotation_Checkbox->OnCheckStateChanged.AddDynamic(this, &UTDS_OptionsMenu::OnCameraBoomRotation_CheckboxChanged);
	}
	if (Cycle_OverallQualityLevel_Decrease_Button && Cycle_OverallQualityLevel_Increase_Button) {
		Cycle_OverallQualityLevel_Decrease_Button->OnClicked.AddDynamic(this, &UTDS_OptionsMenu::OnOverallQualityLevel_DecreaseButtonClicked);
		Cycle_OverallQualityLevel_Increase_Button->OnClicked.AddDynamic(this, &UTDS_OptionsMenu::OnOverallQualityLevel_IncreaseButtonClicked);
	}
}

void UTDS_OptionsMenu::DisplayDefaultSettings()
{
	if (OptionsGIS) {
		EnableCameraBoomOffset_Checkbox->SetCheckedState(OptionsGIS->CameraSettings.bEnableCameraBoomOffset
			? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		EnableCameraBoomRotation_Checkbox->SetCheckedState(OptionsGIS->CameraSettings.bEnableCameraBoomRotation
			? ECheckBoxState::Checked : ECheckBoxState::Unchecked);


		// Update game overal scalability level text
		{
			if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
			{
				OverallGameScalability_QualityLevel =
					static_cast<EQualityLevel>(UserSettings->GetOverallScalabilityLevel());
			}
			UpdateOverallQualityText();
		}
	}
}

void UTDS_OptionsMenu::OnBackButtonPressed()
{
	UTDSUserWidget* MainMenuUW = (UTDSUserWidget*)CreateMainMenuUW();
	if (MainMenuUW) {
		MainMenuUW->AddToViewport();
		RemoveFromParent();
	}
}

void UTDS_OptionsMenu::OnSaveSettingsButtonPressed()
{
	if (OptionsGIS) {
		OptionsGIS->CameraSettings = FCameraOptions{ CameraSettings.bEnableCameraBoomOffset,
			CameraSettings.bEnableCameraBoomOffset };
	}

	if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
	{
		int32 Current_OverallGameScalability_Level = static_cast<int32>(OverallGameScalability_QualityLevel);
		if (Current_OverallGameScalability_Level != UserSettings->GetOverallScalabilityLevel())
		{
			UserSettings->SetOverallScalabilityLevel(Current_OverallGameScalability_Level);
			UserSettings->ApplySettings(true);
			UserSettings->SaveSettings();
		}
	}
}

void UTDS_OptionsMenu::OnCameraBoomOffset_CheckboxChanged(bool bIsChecked)
{
	CameraSettings.bEnableCameraBoomOffset = bIsChecked;
}

void UTDS_OptionsMenu::OnCameraBoomRotation_CheckboxChanged(bool bIsChecked)
{
	CameraSettings.bEnableCameraBoomRotation = bIsChecked;
}

void UTDS_OptionsMenu::OnOverallQualityLevel_DecreaseButtonClicked()
{
	OverallGameScalability_QualityLevel = static_cast<EQualityLevel>(
		FMath::Clamp(static_cast<int32>(OverallGameScalability_QualityLevel) - 1,
			0,
			static_cast<int32>(EQualityLevel::EPIC)
		));
	UpdateOverallQualityText();
}

void UTDS_OptionsMenu::OnOverallQualityLevel_IncreaseButtonClicked()
{
	OverallGameScalability_QualityLevel = static_cast<EQualityLevel>(
		FMath::Clamp(static_cast<int32>(OverallGameScalability_QualityLevel) + 1,
			0,
			static_cast<int32>(EQualityLevel::EPIC)
		));
	UpdateOverallQualityText();
}

void UTDS_OptionsMenu::UpdateOverallQualityText()
{
	FText OverallQuality_DisplayName = StaticEnum<EQualityLevel>()->GetDisplayNameTextByValue((int64)OverallGameScalability_QualityLevel);
	OverallQuality_Text->SetText(OverallQuality_DisplayName);
}

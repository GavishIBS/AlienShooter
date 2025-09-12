// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_OptionsMenu.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Game/Subsystems/OptionsGameInstanceSubsystem.h"

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
}

void UTDS_OptionsMenu::DisplayDefaultSettings()
{
	if (OptionsGIS) {
		EnableCameraBoomOffset_Checkbox->SetCheckedState(OptionsGIS->CameraSettings.bEnableCameraBoomOffset
			? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		EnableCameraBoomRotation_Checkbox->SetCheckedState(OptionsGIS->CameraSettings.bEnableCameraBoomRotation
			? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
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
}

void UTDS_OptionsMenu::OnCameraBoomOffset_CheckboxChanged(bool bIsChecked)
{
	CameraSettings.bEnableCameraBoomOffset = bIsChecked;
}

void UTDS_OptionsMenu::OnCameraBoomRotation_CheckboxChanged(bool bIsChecked)
{
	CameraSettings.bEnableCameraBoomOffset = bIsChecked;
}

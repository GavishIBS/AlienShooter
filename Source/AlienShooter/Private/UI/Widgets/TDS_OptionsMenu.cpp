// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_OptionsMenu.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Game/Subsystems/OptionsGameInstanceSubsystem.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/Slider.h"
#include "HAL/PlatformApplicationMisc.h"
#include "GenericPlatform/GenericApplication.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/Engine.h"

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

	{// get monitors info
		FDisplayMetrics DisplayMetrics;
		FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
		NumMonitors = DisplayMetrics.MonitorInfo.Num();
		FVector2D WindowPos;
		if (GEngine && GEngine->GameViewport)
		{
			TSharedPtr<SWindow> Window = GEngine->GameViewport->GetWindow();
			if (Window.IsValid())
			{
				WindowPos = Window->GetPositionInScreen();
			}
		}
		WindowPos = FVector2D::ZeroVector;
		for (int32 i = 0; i < DisplayMetrics.MonitorInfo.Num(); i++)
		{
			const FMonitorInfo& Monitor = DisplayMetrics.MonitorInfo[i];
			const FPlatformRect& Rect = Monitor.DisplayRect;

			if (WindowPos.X >= Rect.Left && WindowPos.X < Rect.Right &&
				WindowPos.Y >= Rect.Top && WindowPos.Y < Rect.Bottom)
			{
				CurrentMonitorIndex = i;
			}
		}
		CurrentMonitorIndex = 0;
	}

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
	if (Cycle_Monitor_Decrease_Button && Cycle_Monitor_Increase_Button) {
		Cycle_Monitor_Decrease_Button->OnClicked.AddDynamic(this, &UTDS_OptionsMenu::OnMonitor_DecreaseButtonClicked);
		Cycle_Monitor_Increase_Button->OnClicked.AddDynamic(this, &UTDS_OptionsMenu::OnMonitor_IncreaseButtonClicked);
	}
	if (MusicVolumeSlider && EffectsVolumeSlider && DialoguesVolumeSlider) {
		MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UTDS_OptionsMenu::OnMusicVolumeChanged);
		EffectsVolumeSlider->OnValueChanged.AddDynamic(this, &UTDS_OptionsMenu::OnEffectsVolumeChanged);
		DialoguesVolumeSlider->OnValueChanged.AddDynamic(this, &UTDS_OptionsMenu::OnDialoguesVolumeChanged);
	}
	if (BrightnessSlider) {
		BrightnessSlider->OnValueChanged.AddDynamic(this, &UTDS_OptionsMenu::OnBrightnessChanged);
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

		// Update monitor index text
		{
			MonitorIndex_Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentMonitorIndex)));
		}

		MusicVolumeSlider->SetValue(OptionsGIS->MusicVolume);
		EffectsVolumeSlider->SetValue(OptionsGIS->EffectsVolume);
		DialoguesVolumeSlider->SetValue(OptionsGIS->DialoguesVolume);
		BrightnessSlider->SetValue(OptionsGIS->BrightnessAdditiveValue);
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

	// Apply monitor index
	if (GEngine && GEngine->GameViewport) {

		//int MonitorNumber = CurrentMonitorIndex + 1;
		//FParse::Value(FCommandLine::Get(), L"monitor=", MonitorNumber);
		FDisplayMetrics DisplayMetrics;
		FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);

		int8 MonitorIndex = CurrentMonitorIndex;
		int32 CurrentMonitorWidth = DisplayMetrics.MonitorInfo[MonitorIndex].NativeWidth;

		float WidthPosition = (MonitorIndex)*DisplayMetrics.PrimaryDisplayWidth - CurrentMonitorWidth;

		float HeightPosition = 0.0f;

		FVector2D WindowPosition = FVector2D(WidthPosition, 0.f);
		GEngine->GameViewport->GetWindow()->MoveWindowTo(WindowPosition);
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

void UTDS_OptionsMenu::OnMonitor_DecreaseButtonClicked()
{
	CurrentMonitorIndex = FMath::Clamp(CurrentMonitorIndex - 1, 0, NumMonitors - 1);
	MonitorIndex_Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentMonitorIndex)));
}

void UTDS_OptionsMenu::OnMonitor_IncreaseButtonClicked()
{
	CurrentMonitorIndex = FMath::Clamp(CurrentMonitorIndex + 1, 0, NumMonitors - 1);
	MonitorIndex_Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentMonitorIndex)));
}

void UTDS_OptionsMenu::UpdateOverallQualityText()
{
	FText OverallQuality_DisplayName = StaticEnum<EQualityLevel>()->GetDisplayNameTextByValue((int64)OverallGameScalability_QualityLevel);
	OverallQuality_Text->SetText(OverallQuality_DisplayName);
}

void UTDS_OptionsMenu::OnMusicVolumeChanged(float Value)
{
	if (OptionsGIS) {
		OptionsGIS->MusicVolume = Value;
	}
}

void UTDS_OptionsMenu::OnEffectsVolumeChanged(float Value)
{
	if (OptionsGIS) {
		OptionsGIS->EffectsVolume = Value;
	}
}

void UTDS_OptionsMenu::OnDialoguesVolumeChanged(float Value)
{
	if (OptionsGIS) {
		OptionsGIS->DialoguesVolume = Value;
	}
}

void UTDS_OptionsMenu::OnBrightnessChanged(float Value)
{
	if (OptionsGIS) {
		OptionsGIS->BrightnessAdditiveValue = Value;
	}
}

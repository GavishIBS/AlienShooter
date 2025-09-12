// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "OptionTypes.h"
#include "TDS_OptionsMenu.generated.h"

class UButton;
class UTDS_MainMenu;
class UCheckBox;
class UOptionsGameInstanceSubsystem;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_OptionsMenu : public UTDSUserWidget
{
	GENERATED_BODY()
	
public:
	UTDS_OptionsMenu() = default;
	UTDS_OptionsMenu(const FObjectInitializer& ObjectInitializer);

protected:
	UOptionsGameInstanceSubsystem* OptionsGIS;

	FCameraOptions CameraSettings{true, true};

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void BindCallbacks();

	virtual void DisplayDefaultSettings();

	UFUNCTION(BlueprintImplementableEvent)
	UTDS_MainMenu* CreateMainMenuUW();

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SaveSettingsButton;   // saves and applies the new and updated settings

	UPROPERTY(meta = (BindWidget))
	UCheckBox* EnableCameraBoomOffset_Checkbox;
	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* EnableCameraBoomRotation_Checkbox;

	UFUNCTION()
	void OnBackButtonPressed();
	
	UFUNCTION()
	void OnSaveSettingsButtonPressed();

	UFUNCTION()
	void OnCameraBoomOffset_CheckboxChanged(bool bIsChecked);
	
	UFUNCTION()
	void OnCameraBoomRotation_CheckboxChanged(bool bIsChecked);
};

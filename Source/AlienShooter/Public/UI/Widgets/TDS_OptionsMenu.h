// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_OptionsMenu.generated.h"

class UButton;
class UTDS_MainMenu;

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
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void BindCallbacks();

	UFUNCTION(BlueprintImplementableEvent)
	UTDS_MainMenu* CreateMainMenuUW();

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	
	UFUNCTION()
	void OnBackButtonPressed();
};

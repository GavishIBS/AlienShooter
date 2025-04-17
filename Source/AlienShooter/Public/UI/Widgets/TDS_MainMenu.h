// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_MainMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_MainMenu : public UTDSUserWidget
{
	GENERATED_BODY()
	
public:
	UTDS_MainMenu() = default;
	UTDS_MainMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void BindCallbacks();

	UPROPERTY(meta = (BindWidget))
	UButton* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitGameButton;

	UFUNCTION()
	void OnNewGameButtonPressed();
	
	UFUNCTION()
	void OnContinueGameButtonPressed();
	
	UFUNCTION()
	void OnOptionsButtonPressed();
	
	UFUNCTION()
	void OnExitGameButtonPressed();
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_OptionsMenu.h"
#include "Components/Button.h"

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
	BindCallbacks();
}

void UTDS_OptionsMenu::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_OptionsMenu::BindCallbacks()
{
	if (BackButton) {
		BackButton->OnClicked.AddDynamic(this, &UTDS_OptionsMenu::OnBackButtonPressed);
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

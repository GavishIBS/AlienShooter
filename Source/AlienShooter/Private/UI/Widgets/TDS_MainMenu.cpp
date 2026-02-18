// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_MainMenu.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"
#include "SaveGameObjects/GameDataSaveObject.h"

// TEST // DEBUG
#include "Kismet/KismetSystemLibrary.h"

UTDS_MainMenu::UTDS_MainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_MainMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDS_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	BindCallbacks();
}

void UTDS_MainMenu::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_MainMenu::BindCallbacks()
{
	if (NewGameButton) {
		NewGameButton->OnClicked.AddDynamic(this, &UTDS_MainMenu::OnNewGameButtonPressed);
	}
	if (ContinueGameButton) {
		ContinueGameButton->OnClicked.AddDynamic(this, &UTDS_MainMenu::OnContinueGameButtonPressed);
	}
	if (OptionsButton) {
		OptionsButton->OnClicked.AddDynamic(this, &UTDS_MainMenu::OnOptionsButtonPressed);
	}
	if (ExitGameButton) {
		ExitGameButton->OnClicked.AddDynamic(this, &UTDS_MainMenu::OnExitGameButtonPressed);
	}
}

void UTDS_MainMenu::OnNewGameButtonPressed()
{
	UGameplayStatics::OpenLevel(this, FName("1st"));
}

void UTDS_MainMenu::OnContinueGameButtonPressed()
{
	// TODO: convert to async loading using delegate

	if (UGameDataSaveObject* LoadedGameInst = Cast<UGameDataSaveObject>(UGameplayStatics::LoadGameFromSlot(FString("GameDataSaveSlot"), 0)))
	{
		FName LevelName = "1st";
		switch (LoadedGameInst->NumberOfLevelsCompleted)
		{
		case 1:
			LevelName = "2nd";
			break;
		case 2:
			LevelName = "3rd";
			break;
		case 3:
			LevelName = "4th";
			break;
		case 4:
			LevelName = "5thBoss";
			break;
		case 5:
			LevelName = "6th";
			break;
		case 6:
			LevelName = "7th";
			break;
		case 7:
			LevelName = "8th";
			break;
		case 8:
			LevelName = "9th";
			break;
		case 9:
			LevelName = "MainMenuLevel";
			break;
		default:
			break;
		}
		UGameplayStatics::OpenLevel(this, LevelName);
	}
}

void UTDS_MainMenu::OnOptionsButtonPressed()
{
	UTDSUserWidget* OptionsMenuUW = (UTDSUserWidget*)CreateOptionsMenuUI();
	if (OptionsMenuUW) {
		OptionsMenuUW->AddToViewport();
		RemoveFromParent();
	}
}

void UTDS_MainMenu::OnExitGameButtonPressed()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
}

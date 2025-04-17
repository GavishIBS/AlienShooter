// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_MainMenu.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"
#include "SaveGameObjects/GameDataSaveObject.h"

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
		default:
			break;
		}
		UGameplayStatics::OpenLevel(this, LevelName);
	}

	/*
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	if (!BaseCharacter) return false;

	if (UMainSaveGameObject* LoadedGameInst = Cast<UMainSaveGameObject>(UGameplayStatics::LoadGameFromSlot(FString("MainSaveSlot"), 0)))
	{
		BaseCharacter->SetPlayerName(LoadedGameInst->PlayerName);
		if (UCombatComponent* CombatC = BaseCharacter->GetComponentByClass<UCombatComponent>())
		{
			CombatC->Weapons = LoadedGameInst->Weapons;
		}
		if (UInventoryComponent* InventoryC = BaseCharacter->GetComponentByClass<UInventoryComponent>())
		{
			InventoryC->SetContent(LoadedGameInst->InventoryContent);
		}
		return true;
	}

	return false;
	*/
}

void UTDS_MainMenu::OnOptionsButtonPressed()
{
}

void UTDS_MainMenu::OnExitGameButtonPressed()
{
}

// // Copyright (c) 2024 Gavish Bansal// // All rights reserved.


#include "Controllers/ShooterPlayerController.h"
#include "Characters/BaseCharacter.h"
#include "SaveGameObjects/MainSaveGameObject.h"
#include "Kismet/GameplayStatics.h"

AShooterPlayerController::AShooterPlayerController()
{
}

bool AShooterPlayerController::SavePlayerData()
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	if (!BaseCharacter) return false;

	if (UMainSaveGameObject* SaveGameInst = Cast<UMainSaveGameObject>(UGameplayStatics::CreateSaveGameObject(UMainSaveGameObject::StaticClass())))
	{
		//SaveGameInst->PlayerName = BaseCharacter->GetPlayerName();
		SaveGameInst->PlayerName = FString("NewPlayerName");
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInst, TEXT("MainSaveSlot"), 0);
		return true;
	}

	return false;
}

bool AShooterPlayerController::LoadPlayerData()
{
	// TODO: convert to async loading using delegate

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	if (!BaseCharacter) return false;

	if (UMainSaveGameObject* LoadedGameInst = Cast<UMainSaveGameObject>(UGameplayStatics::LoadGameFromSlot(FString("MainSaveSlot"), 0)))
	{
		BaseCharacter->SetPlayerName(LoadedGameInst->PlayerName);
		return true;
	}

	return false;
}

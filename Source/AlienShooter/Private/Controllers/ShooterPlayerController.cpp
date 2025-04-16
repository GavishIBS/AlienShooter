// // Copyright (c) 2024 Gavish Bansal// // All rights reserved.


#include "Controllers/ShooterPlayerController.h"
#include "Characters/BaseCharacter.h"
#include "SaveGameObjects/MainSaveGameObject.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CombatComponent.h"
#include "Components/InventoryComponent.h"

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
		if (UCombatComponent* CombatC = BaseCharacter->GetComponentByClass<UCombatComponent>())
		{
			SaveGameInst->Weapons = CombatC->Weapons;
		}
		if (UInventoryComponent* InventoryC = BaseCharacter->GetComponentByClass<UInventoryComponent>())
		{
			SaveGameInst->InventoryContent = InventoryC->GetContent();
		}

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
}

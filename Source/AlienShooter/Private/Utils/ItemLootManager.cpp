// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Utils/ItemLootManager.h"
#include "Interfaces/LootInterface.h"
#include "Kismet/GameplayStatics.h"

AItemLootManager::AItemLootManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItemLootManager::AddLoot()
{
	Super::AddLoot();

	for (auto It = Items_LootData.CreateIterator(); It; ++It)
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(this, It.Key(), OutActors);
		for (auto& Actor : OutActors)
		{
			if (ILootInterface* LootInterface = Cast<ILootInterface>(Actor))
			{
				LootInterface->SetLootData(It.Value());
			}
			else {
				break;
			}
		}
	}
}

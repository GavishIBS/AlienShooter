// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Utils/WorldLootManager.h"
#include "Targets/LootTargetPoint.h"
#include "Kismet/GameplayStatics.h"

AWorldLootManager::AWorldLootManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWorldLootManager::AddLoot()
{
	Super::AddLoot();

	if (!TargetPointClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	TArray<AActor*> Targets;
	UGameplayStatics::GetAllActorsOfClass(this, TargetPointClass, Targets);

	while (Targets.Num() > 0)
	{
		const FVector TargetLocation = Targets[0]->GetTargetLocation();
		const FRotator TargetRotation = Targets[0]->GetActorRotation();
		const int RandomLootSelectionIndex = FMath::RandRange(0, World_LootData.Num() - 1);
		
		if (!World_LootData[RandomLootSelectionIndex].LootItemClass) continue;
		if (World_LootData[RandomLootSelectionIndex].Quantity <= 0)
		{
			World_LootData.RemoveAt(RandomLootSelectionIndex);
			continue;
		}

		const FTransform SpawnTransform = FTransform(TargetRotation, TargetLocation, FVector(1.f, 1.f, 1.f));
		const FActorSpawnParameters ActorSpawmParams;
		AActor* ActorSpawned = World->SpawnActor<AActor>(World_LootData[RandomLootSelectionIndex].LootItemClass, SpawnTransform, ActorSpawmParams);
		SpawnedLoot_Cache.Add(ActorSpawned);

		World_LootData[RandomLootSelectionIndex].Quantity--;
		if (World_LootData[RandomLootSelectionIndex].Quantity <= 0)
		{
			World_LootData.RemoveAt(RandomLootSelectionIndex);
		}

		Targets.RemoveAt(0);
	}
}

void AWorldLootManager::ClearLoot()
{
	for (auto& Item : SpawnedLoot_Cache)
	{
		if (Item)
			Item->Destroy();
	}
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/AISpawnerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "AI/Pawns/BaseAICharacter.h"

AAISpawnerBase::AAISpawnerBase()
{
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(GetRootComponent());
}

bool AAISpawnerBase::StartSpawning()
{
	if (SpawnPoint)
	{
		GetWorldTimerManager().SetTimer(SpawnerTickTimerHandle, this, &AAISpawnerBase::SpawnTick, SpawnTimerTickRate, true, SpawnTimerTickRate);
		return true;
	}
	return false;
}

void AAISpawnerBase::SpawnTick()
{
	if (SpawnData.Num() == 0) { Destroy(); return; }

	int RandomSelectionIndex = UKismetMathLibrary::RandomIntegerInRange(0, SpawnData.Num() - 1);
	FAISpawnData* SpawnDataSelection = &SpawnData[RandomSelectionIndex];
	if (!SpawnDataSelection) return;

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters ActorSpawnParamerts;
		ActorSpawnParamerts.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(SpawnPoint->GetComponentLocation(),
			SpawnPoint->GetComponentRotation());
		ABaseAICharacter* AICharacter = World->SpawnActor<ABaseAICharacter>(SpawnDataSelection->AIClass,
			SpawnTransform, ActorSpawnParamerts);
		if (AICharacter)
			AICharacter->SpawnDefaultController();
		SpawnDataSelection->Count--;
		if (SpawnDataSelection->Count <= 0)
			SpawnData.RemoveAt(RandomSelectionIndex);
	}
}

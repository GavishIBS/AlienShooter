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

void AAISpawnerBase::MultiplySpawnCount(const float Factor)
{
	for (FAISpawnData& Data : SpawnData) {
		const float NewSpawnCount = (float)Data.Count * Factor;
		Data.Count = (int)NewSpawnCount;
	}
}

void AAISpawnerBase::StartSpawningWrapper()
{
	StartSpawning();
}

bool AAISpawnerBase::StartSpawning()
{
	bool ReturnValue = false;

	if (SpawnDelay > 0.f) {
		FTimerHandle SpawnDelayTimerHandle;
		GetWorldTimerManager().SetTimer(SpawnDelayTimerHandle, FTimerDelegate::CreateLambda([&] {
			if (SpawnPoint)
			{
				GetWorldTimerManager().SetTimer(SpawnerTickTimerHandle, this, &AAISpawnerBase::SpawnTick, SpawnTimerTickRate, true, SpawnTimerTickRate);
				ReturnValue = true;
			}
			}), SpawnDelay, false, SpawnDelay);
	}
	else {
		GetWorldTimerManager().SetTimer(SpawnerTickTimerHandle, this, &AAISpawnerBase::SpawnTick, SpawnTimerTickRate, true, SpawnTimerTickRate);
		ReturnValue = true;
	}

	return ReturnValue;
}

void AAISpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	if (bSpawnOnBeginPlay) {
		StartSpawning();
	}
}

void AAISpawnerBase::SpawnTick()
{
	if (SpawnData.Num() == 0) { Destroy(); return; }

	int RandomSelectionIndex = UKismetMathLibrary::RandomIntegerInRange(0, SpawnData.Num() - 1);
	FAISpawnData* SpawnDataSelection = &SpawnData[RandomSelectionIndex];
	if (!SpawnDataSelection) return;

	if (UWorld* World = GetWorld())
	{
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
		if (SpawnPoints.Num() > 0) {
			int32 RandomSpawnPointIndex = FMath::FRandRange(0.f, SpawnPoints.Num());
			AActor* Point = (AActor*)SpawnPoints[RandomSpawnPointIndex];
			if (Point) {
				SpawnTransform = UKismetMathLibrary::MakeTransform(Point->GetActorLocation(), Point->GetActorRotation());
			}
		}

		FActorSpawnParameters ActorSpawnParamerts;
		ActorSpawnParamerts.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ABaseAICharacter* AICharacter = World->SpawnActor<ABaseAICharacter>(SpawnDataSelection->AIClass,
			SpawnTransform, ActorSpawnParamerts);
		if (AICharacter)
			AICharacter->SpawnDefaultController();
		SpawnDataSelection->Count--;
		if (SpawnDataSelection->Count <= 0)
			SpawnData.RemoveAt(RandomSelectionIndex);
	}
}

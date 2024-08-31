// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnerBase.generated.h"

class ABaseAICharacter;

USTRUCT(BlueprintType)
struct FAISpawnData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Data");
	TSubclassOf<ABaseAICharacter> AIClass = nullptr;
	UPROPERTY(EditAnywhere, Category = "Data");
	int Count = 10;
};

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AAISpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:
	friend class ASpawnStarter;
	AAISpawnerBase();

protected:
	bool StartSpawning();
	void SpawnTick();

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TObjectPtr<USceneComponent> SpawnPoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Data");
	TArray<FAISpawnData> SpawnData;

	int NumSpawned = 0;   // Number of AI characters spawned through this spawner.

	UPROPERTY(EditAnywhere, Category = "Properties")
	float SpawnTimerTickRate = 0.085f;

	FTimerHandle SpawnerTickTimerHandle;
};

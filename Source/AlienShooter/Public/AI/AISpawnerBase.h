// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnerBase.generated.h"

class ABaseAICharacter;
class ATargetPoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIEnemySpawned, AController*, SpawnedEnemyControlled);

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

	UFUNCTION(BlueprintCallable)   // TEST
	void MultiplySpawnCount(const float Factor);    // TEST

	UFUNCTION(BlueprintCallable)
	void StartSpawningWrapper();

	UPROPERTY(BlueprintAssignable)
	FOnAIEnemySpawned OnAIEnemySpawned;

protected:
	virtual void BeginPlay() override;
	bool StartSpawning();
	void SpawnTick();

	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bSpawnOnBeginPlay = false;
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	float SpawnDelay = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TObjectPtr<USceneComponent> SpawnPoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Data");
	TArray<FAISpawnData> SpawnData;

	UPROPERTY(EditAnywhere, Category=Data)
	TArray<ATargetPoint*> SpawnPoints;

	int NumSpawned = 0;   // Number of AI characters spawned through this spawner.

	bool bAlreadySpawned = false;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float SpawnTimerTickRate = 0.085f;

	FTimerHandle SpawnerTickTimerHandle;
};

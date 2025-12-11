// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnStarter.generated.h"

class AAISpawnerBase;
class UBoxComponent;

UCLASS()
class ALIENSHOOTER_API ASpawnStarter : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnStarter();

	void DisableSpawnStarter();
	void EnableSpawnStarter();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<AAISpawnerBase> Spawner = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> OverlapBox = nullptr;

public:
	FORCEINLINE TObjectPtr<AAISpawnerBase> GetAISpawnerRef() const { return Spawner; }
};

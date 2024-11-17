// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActor.generated.h"

UCLASS()
class ALIENSHOOTER_API APooledActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APooledActor();

protected:
	virtual void BeginPlay() override;

private:
	bool bInUse = false;

	bool SpawnedByPool = false;

public:
	FORCEINLINE bool IsInUse() const { return bInUse; }
	FORCEINLINE bool WasSpawnedByPool() const { return SpawnedByPool; }

	virtual void SetInUse(const bool InValue);
	FORCEINLINE void SetSpawnedByPool(const bool InValue) { SpawnedByPool = InValue; }
};

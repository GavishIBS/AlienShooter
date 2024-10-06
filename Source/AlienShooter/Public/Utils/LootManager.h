// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootManager.generated.h"

UCLASS()
class ALIENSHOOTER_API ALootManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ALootManager();

protected:
	UFUNCTION(CallInEditor, Category = "Functions")
	virtual void AddLoot();
};

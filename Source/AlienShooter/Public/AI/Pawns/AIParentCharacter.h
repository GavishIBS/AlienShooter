// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameCharacter.h"
#include "AIParentCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AAIParentCharacter : public AGameCharacter
{
	GENERATED_BODY()
	
public:
	AAIParentCharacter();

protected:
	virtual void BeginPlay() override;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIParentCharacter.h"
#include "BossCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ABossCharacter : public AAIParentCharacter
{
	GENERATED_BODY()
	
public:
	ABossCharacter();

protected:
	virtual void BeginPlay() override;
};

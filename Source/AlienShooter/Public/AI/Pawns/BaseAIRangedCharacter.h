// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Pawns/BaseAICharacter.h"
#include "BaseAIRangedCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ABaseAIRangedCharacter : public ABaseAICharacter
{
	GENERATED_BODY()
	
public:
	ABaseAIRangedCharacter();

protected:
	virtual void BeginPlay() override;
};

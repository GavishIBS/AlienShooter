// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/AI_Types.h"
#include "AIControllerBase.generated.h"

class ABaseAIPawn;
class ABaseAICharacter;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	friend ABaseAIPawn;
	friend ABaseAICharacter;
	AAIControllerBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	FAIProperties AIProperties = FAIProperties();
	
	// Behaviour params
	TObjectPtr<ACharacter> TargetCharacter = nullptr;

public:
	FORCEINLINE ACharacter* GetTargetCharacter() const {return TargetCharacter;}
	FORCEINLINE FAIProperties GetAIProperties() const {return AIProperties;}

	FORCEINLINE void SetTargetCharacter(ACharacter* InCharacter) {TargetCharacter = InCharacter;}
	FORCEINLINE void SetHealth(const float& NewHealth) {AIProperties.Health = NewHealth;}
};

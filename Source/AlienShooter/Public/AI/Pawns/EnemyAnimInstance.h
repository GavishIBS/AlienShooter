// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class ABaseAICharacter;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ownership | References")
	TObjectPtr<ABaseAICharacter> OwnerAICharacter = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Params")
	float GroundSpeed = 0.f;
};

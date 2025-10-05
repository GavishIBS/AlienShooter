// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Pawns/BaseAICharacter.h"
#include "BaseAIRangedCharacter.generated.h"

class AEnemyAttackProjectile;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ABaseAIRangedCharacter : public ABaseAICharacter
{
	GENERATED_BODY()
	
public:
	ABaseAIRangedCharacter();

	UFUNCTION(BlueprintCallable)
	void SpawnRangedAttack();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Ranged | Properties")
	TSubclassOf<AEnemyAttackProjectile> Attack_ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Ranged | Properties")
	FName RangedAttackSpawnSocketName;
};

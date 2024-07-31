// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/AIInterface.h"
#include "Interfaces/HitInterface.h"
#include "BaseAICharacter.generated.h"

class AAIControllerBase;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ABaseAICharacter : public ACharacter, public IHitInterface, public IAIInterface
{
	GENERATED_BODY()

public:
	ABaseAICharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	
	/* Interface begin */
	virtual float GetHit(const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser) override;
	/* Interface end */

protected:
	virtual void BeginPlay() override;

	void InitCallbacks();

	UFUNCTION()
	void OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	TObjectPtr<AAIControllerBase> AIController;
};

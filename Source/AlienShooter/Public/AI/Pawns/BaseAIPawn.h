// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/AI_Types.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/AIInterface.h"
#include "Interfaces/HitInterface.h"
#include "BaseAIPawn.generated.h"

class AAIControllerBase;
class ABaseAIPawn;

UCLASS()
class ALIENSHOOTER_API ABaseAIPawn : public APawn, public IHitInterface, public IAIInterface
{
	GENERATED_BODY()

public:
	ABaseAIPawn();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	
	/* Interface begin */
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser) override;
	/* Interface end */

protected:
	virtual void BeginPlay() override;

	void InitCallbacks();

	UFUNCTION()
	void OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	TObjectPtr<AAIControllerBase> AIController;
};

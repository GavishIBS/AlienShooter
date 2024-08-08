// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/AI_Types.h"
#include "GameFramework/Character.h"
#include "Interfaces/AIInterface.h"
#include "Interfaces/HitInterface.h"
#include "BaseAICharacter.generated.h"

class AAIControllerBase;
class UCollisionTraceComponent;

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
	virtual void PostInitializeComponents() override;
	
	/* Interface begin */
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser) override;

	UFUNCTION(BlueprintCallable, Category=Attacking)
	virtual void StartAttackTrace() override;

	UFUNCTION(BlueprintCallable, Category=Attacking)
	virtual void EndAttackTrace() override;

	virtual bool IsInAttackRange() override;

	UFUNCTION(BlueprintCallable, Category=Attacking)
	virtual void OnEndAttack() override;
	/* Interface end */

protected:
	virtual void BeginPlay() override;
	
	/* Interface begin */
	virtual void AttackTrace() override;
	/* Interface end */

	void InitCallbacks();

	UFUNCTION()
	void OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool CanAttack() const;
	void Attack() const;

	UPROPERTY(EditDefaultsOnly, Category=Animations)
	FAIAnimations AIAnimations;

	FTimerHandle AttackTraceTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category=Attacking)
	float AttackTraceRate = 0.1f;
	
	TObjectPtr<AAIControllerBase> AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UCollisionTraceComponent> CollisionTraceComponent;
};

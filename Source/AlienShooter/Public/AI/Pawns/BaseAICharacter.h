// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/AI_Types.h"
#include "../../GameCharacter.h"
#include "Interfaces/AIInterface.h"
#include "Interfaces/HitInterface.h"
#include "SpecialWeaponTypes.h"
#include "BaseAICharacter.generated.h"

class AAIControllerBase;
class UCollisionTraceComponent;
class UMaterialInstance;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ABaseAICharacter : public AGameCharacter, public IHitInterface, public IAIInterface
{
	GENERATED_BODY()

public:
	ABaseAICharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;
	
	/* Interface begin */
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor,
		AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType = nullptr) override;
	virtual void GetSpecialHit(const FVector& HitLocation, const FSpecialEffectData& SpecialEffectData,
		const float& RandomizationFactor, AActor* InDamageCauser) override;

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

	UFUNCTION()
	virtual void OnDeathCallback(AActor* Killer);

	bool CanAttack() const;
	void Attack() const;

	UFUNCTION(BlueprintCallable, Category = "Death")
	void ShouldDestroy();

	virtual void StopwatchTick() override;

	virtual void ClearAllTimers();

	UPROPERTY(EditDefaultsOnly, Category=Animations)
	FAIAnimations AIAnimations;

	FTimerHandle AttackTraceTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category=Attacking)
	float AttackTraceRate = 0.1f;
	
	TObjectPtr<AAIControllerBase> AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UCollisionTraceComponent> CollisionTraceComponent;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TArray<TObjectPtr<UMaterialInstance>> DeathDecalMaterialInstances = {};

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FVector DeathDecalSize = FVector(100.f, 100.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float DeathDecalLifeSpawn = 5.f;

	FAcquiredSpecialEffectData AcquiredSpecialEffectData;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (ClampMin = "0", ClampMax = "1000"))
	float SpeedRandomizationRange = 50.f;

	// Dumbness logic begin
	UPROPERTY(EditAnywhere, Category = "Properties | Dumbness")
	bool bCanForgetTarget = false;
	
	UPROPERTY(EditAnywhere, Category = "Properties | Dumbness")
	bool bOnlyForgetWhenNoPathFound = false;
	
	UPROPERTY(EditAnywhere, Category = "Properties | Dumbness")
	float PathCheckRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "Properties | Dumbness")
	float ForgetTargetRate = 15.f;    // Amount of time after which AI should forget target

	UPROPERTY(EditAnywhere, Category = "Properties | Dumbness")
	float TargetForgetMinRange = 1500.f;  // Target should be atlest 'x' units away to forget
	
	UPROPERTY(EditAnywhere, Category = "Properties | Dumbness")
	float ForgetTargetTime = 8.f;        // Amount of time for which AI forgets target
	
	UPROPERTY(EditAnywhere, Category = "Properties | Dumbness")
	bool bStopOnTargetForget = false;

	UPROPERTY(EditAnywhere, Category = "Properties | Dumbness", meta = (ClampMin="10"))
	float NoTargetMoveRandomPointArea = 200.f;

	UPROPERTY(EditAnywhere, Category = "Properties | Dumbness")
	float NoTargetRandomMovementResetRate = 1.f;

	FTimerHandle ForgetTargetTimerHandle;
	FTimerHandle NoTargetRandomMoveTickTimerHandle;

	bool CanForgetTarget();
	void ForgetTarget();
	void ReaquireTarget();
	// Dumbness logic end

private:
	// Timer handles
	FTimerHandle CheckCanAttackTimerHandle;
	FTimerHandle ReaquireTargetTimerHandle;
};

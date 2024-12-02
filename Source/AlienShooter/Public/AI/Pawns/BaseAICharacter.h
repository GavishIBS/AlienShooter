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
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;
	
	/* Interface begin */
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass) override;
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

	UPROPERTY(EditDefaultsOnly, Category=Animations)
	FAIAnimations AIAnimations;

	FTimerHandle AttackTraceTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category=Attacking)
	float AttackTraceRate = 0.1f;
	
	TObjectPtr<AAIControllerBase> AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UCollisionTraceComponent> CollisionTraceComponent;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UMaterialInstance> DeathDecalMaterialInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FVector DeathDecalSize = FVector(100.f, 100.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float DeathDecalLifeSpawn = 5.f;

	FAcquiredSpecialEffectData AcquiredSpecialEffectData;
};

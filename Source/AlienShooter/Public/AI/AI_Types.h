#pragma once

#include "CoreMinimal.h"
#include "AI_Types.generated.h"

USTRUCT(BlueprintType)
struct FAIProperties
{
	GENERATED_USTRUCT_BODY()

class ABaseAIPawn;
class ABaseAICharacter;
	
public:
	UPROPERTY(EditDefaultsOnly, Category=Properties)
	float AcceptanceRadius = 10.f;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	float Health = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category=Properties)
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	float AttackDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	float AttackDamageRandomizationFactor = 4.f;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	float AttackRange = 100.f;

public:
	void SetHealth(const float& NewHealth)
	{
		Health = NewHealth;
	}
};

USTRUCT(BlueprintType)
struct FAIAnimations
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category=Animations)
	TArray<UAnimMontage*> AttackAnimMontages = TArray<UAnimMontage*>();

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> DeathAnimMontage = nullptr;
};

UENUM()
enum EAIState : uint8
{
	EAS_Idle = 0,
	EAS_Attacking = 1
};

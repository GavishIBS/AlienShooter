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

public:
	void SetHealth(const float& NewHealth)
	{
		Health = NewHealth;
	}
};

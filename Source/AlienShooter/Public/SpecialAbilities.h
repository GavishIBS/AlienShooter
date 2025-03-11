// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SpecialAbilities.generated.h"

UENUM()
enum class EEffectApplicationPolicy
{
	EEAP_None = 0,     // Don't apply
	EEAP_Instant = 1,
	EEAP_Duration = 2,
	EEAP_Infinite = 3     // Gets removed when overriden by some other effect
};

USTRUCT(BlueprintType)
struct FSpecialSerumEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Data")
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::EEAP_None;

	UPROPERTY(EditAnywhere, Category="Data")
	class UNiagaraSystem* CharacterStateFX = nullptr;

	UPROPERTY(EditAnywhere, Category="Data")
	float Duration = 7.f;    // Duration through which the player can use ability

	UPROPERTY(EditAnywhere, Category="Data")
	float ExecutionDuration = 1.25f;    // Actual use time of a ability
};

class Abilities
{
public:
	Abilities();
	~Abilities();

	static bool Dash(class ACharacter* InCharacter, float ExecutionDuration = 1.25f);
	static void OnDashExecutionEnd();
	static FTimerHandle DashExecutionEndTimerHandle;
	static class ACharacter* Char;

private:

};

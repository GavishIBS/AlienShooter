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
	class UNiagaraSystem* CharacterStateFX;

	UPROPERTY(EditAnywhere, Category="Data")
	float Duration = 7.f;
};

bool Dash(class ACharacter* InCharacter);

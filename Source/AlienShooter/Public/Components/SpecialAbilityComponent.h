// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpecialAbilities.h"
#include "SpecialAbilityComponent.generated.h"

class AShooterCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API USpecialAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USpecialAbilityComponent();
	void Init_Component();
	void UseAbility();

protected:
	virtual void BeginPlay() override;

	bool (*EffectImplementation)(ACharacter*, float);
	FSpecialSerumEffectData EffectData;

protected:
	virtual void OnAbilityDataSet();

	void RemoveAbility();

	TObjectPtr<AShooterCharacter> OwnerAsShooterCharacter;

private:
	FTimerHandle DurationAbilityTimerHandle;

public:
	//FORCEINLINE void SetEffectImplementation(bool(*InImplementation)(ACharacter*)) { EffectImplementation = InImplementation; }
	//FORCEINLINE void SetEffectData(const FSpecialEffectData& InEffectData) { EffectData = InEffectData; }
	FORCEINLINE void SetAbilityData(bool(*InImplementation)(ACharacter*, float), const FSpecialSerumEffectData& InEffectData) {
		EffectImplementation = InImplementation;
		EffectData = InEffectData;
		OnAbilityDataSet();
	}
};

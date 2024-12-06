// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/SpecialAbilityComponent.h"
#include "Characters/ShooterCharacter.h"

USpecialAbilityComponent::USpecialAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USpecialAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USpecialAbilityComponent::Init_Component()
{
	OwnerAsShooterCharacter = Cast<AShooterCharacter>(GetOwner());
}

void USpecialAbilityComponent::UseAbility()
{
	if (EffectImplementation) {
		EffectImplementation((ACharacter*)OwnerAsShooterCharacter);
	}
}

void USpecialAbilityComponent::OnAbilityDataSet()
{
	if (!OwnerAsShooterCharacter) return;
	if (EffectImplementation == nullptr || EffectData.EffectApplicationPolicy == EEffectApplicationPolicy::EEAP_None) return;

	OwnerAsShooterCharacter->ActivateSpecialAbilityNiagaraEffect(EffectData.CharacterStateFX);
	
	switch (EffectData.EffectApplicationPolicy)
	{
		case EEffectApplicationPolicy::EEAP_Instant:
			UseAbility();
			RemoveAbility();
			break;
		case EEffectApplicationPolicy::EEAP_Duration:
			if (AActor* OwnerActor = GetOwner()) {
				OwnerActor->GetWorldTimerManager().SetTimer(DurationAbilityTimerHandle, this,
					&USpecialAbilityComponent::RemoveAbility, EffectData.Duration, false, EffectData.Duration);
			}
			break;
		case EEffectApplicationPolicy::EEAP_Infinite:
			break;
		default:
			break;
	}
}

void USpecialAbilityComponent::RemoveAbility()
{
	if (OwnerAsShooterCharacter) {
		OwnerAsShooterCharacter->DeactivateSpecialAbilityNiagaraEffect();
	}
	EffectImplementation = nullptr;
	EffectData = FSpecialSerumEffectData();
}

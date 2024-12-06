// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/Usables/SpecialAbilities/SpecialSerumBase.h"
#include "GameFramework/Character.h"
#include "SpecialAbilities.h"
#include "Components/SpecialAbilityComponent.h"

ASpecialSerumBase::ASpecialSerumBase()
{
	PrimaryActorTick.bCanEverTick = false;
	EffectImplementation = Dash;
}

void ASpecialSerumBase::BeginPlay()
{
	Super::BeginPlay();
}

bool ASpecialSerumBase::CanUse(ACharacter* User, FHitResult& HitResult)
{
	return Super::CanUse(User, HitResult);
}

// TODO: Destroy after use (after use animation)

bool ASpecialSerumBase::UseItem(ACharacter* User)
{
	//return ApplyEffect(User);
	//return EffectImplementation(User);
	if (USpecialAbilityComponent* SpecialAbilityComponent = User->FindComponentByClass<USpecialAbilityComponent>()) {
		SpecialAbilityComponent->SetAbilityData(EffectImplementation, EffectData);
		return true;
	}
	return false;
}

/*
bool ASpecialSerumBase::ApplyEffect(ACharacter* InCharacter)
{
	if (!InCharacter) return false;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString("Applying special effect!"));
	InCharacter->LaunchCharacter(InCharacter->GetMesh()->GetForwardVector() * 2500.f, false, false);
	return true;
}
*/

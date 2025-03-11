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
	EffectImplementation = Abilities::Dash;
}

void ASpecialSerumBase::BeginPlay()
{
	Super::BeginPlay();
}

bool ASpecialSerumBase::CanUse(ACharacter* User, FHitResult& HitResult)
{
	return Super::CanUse(User, HitResult);
}

// TODO: Destroy after use (after use UseItemanimation)

AUsableActor* ASpecialSerumBase::UseItem(ACharacter* User)
{
	//return ApplyEffect(User);
	//return EffectImplementation(User);
	if (USpecialAbilityComponent* SpecialAbilityComponent = User->FindComponentByClass<USpecialAbilityComponent>()) {
		SpecialAbilityComponent->SetAbilityData(EffectImplementation, EffectData);
		//Destroy();
		return this;
	}
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Cyan, FString("Failed to find special ability component"));
	Destroy();
	return 0;
}

void ASpecialSerumBase::OnItemUsed()
{
	Super::OnItemUsed();
	Destroy();
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

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/AttributeComponent.h"
#include "Actors/Pickups/Armor.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UAttributeComponent::CanTakeArmor(const EArmorColorCode ArmorColorCode)
{
	if (ArmorEquipped->GetArmorColorCode() == EArmorColorCode::ECC_None) return false;
	if (!ArmorEquipped) return true;
	if (ArmorEquipped->GetArmorColorCode() < ArmorColorCode) return true;
	if (ArmorEquipped->GetArmorColorCode() == ArmorColorCode)
		if (ArmorEquipped->IsDamaged()) return true;
	return false;
}

void UAttributeComponent::TakeArmor(AArmor* InArmor)
{
	ArmorEquipped = InArmor;
}


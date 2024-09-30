// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/AttributeComponent.h"
#include "Actors/Pickups/Armor.h"
#include "GameCharacter.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TakeDamage(const float InDamage)
{
	float BodyDamage = InDamage;
	if (ArmorEquipped)
	{
		float ArmorDamage = ArmorEquipped->GetArmorDamageAbsorption() * InDamage / 100;
		BodyDamage = InDamage - ArmorDamage;
		if (ArmorEquipped->GetArmorHealth() < ArmorDamage)
		{
			BodyDamage += ArmorDamage - ArmorEquipped->GetArmorHealth();
		}
		ArmorEquipped->TakeDamage(ArmorDamage);
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Amor: %f, Body: %f"), ArmorDamage, BodyDamage));
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, FString::Printf(TEXT("AmorHealth: %f, BodyHealth: %f"), ArmorEquipped->GetArmorHealth(), Health));
		if (ArmorEquipped->GetArmorHealth() <= 0.f)
		{
			BreakArmor();
		}
	}
	Health = FMath::Clamp(Health - InDamage, 0.f, BodyDamage);
}

bool UAttributeComponent::CanTakeArmor(const EArmorColorCode ArmorColorCode)
{
	if (!ArmorEquipped) return true;
	if (ArmorEquipped->GetArmorColorCode() == EArmorColorCode::ECC_None) return false;
	if (ArmorEquipped->GetArmorColorCode() < ArmorColorCode) return true;
	if (ArmorEquipped->GetArmorColorCode() == ArmorColorCode)
		if (ArmorEquipped->IsDamaged()) return true;
	return false;
}

void UAttributeComponent::TakeArmor(AArmor* InArmor)
{
	ArmorEquipped = InArmor;
}

void UAttributeComponent::BreakArmor()
{
	if (ArmorEquipped)
	{
		ArmorEquipped->Break();
	}
}


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

void UAttributeComponent::ReInitComponent()
{
	SetHealth(MaxHealth);
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
		ArmorEquipped->AbsorbDamage(ArmorDamage);
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Amor: %f, Body: %f"), ArmorDamage, BodyDamage));
		if (ArmorEquipped->GetArmorHealth() <= 0.f)
		{
			BreakArmor();
		}
	}
	Health = FMath::Clamp(Health - BodyDamage, 0.f, MaxHealth);
	if (ArmorEquipped)
	{
		OnArmorUpdated.Broadcast(ArmorEquipped->GetArmorHealth(), ArmorEquipped->GetArmorMaxHealth(), true);
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, FString::Printf(TEXT("AmorHealth: %f, BodyHealth: %f"), ArmorEquipped->GetArmorHealth(), Health));
	}

	OnHealthUpdated.Broadcast(Health, MaxHealth);
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

bool UAttributeComponent::TakeArmor(AArmor* InArmor)
{
	// Break last armor
	BreakArmor();

	ArmorEquipped = InArmor;
	if (!ArmorEquipped) return false;

	OnArmorUpdated.Broadcast(ArmorEquipped->GetArmorHealth(), ArmorEquipped->GetArmorMaxHealth(), true);

	if (AActor* OwnerActor = GetOwner())
	{
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
		ArmorEquipped->AttachToActor(OwnerActor, AttachmentTransformRules, ArmorEquipped->GetAttachmentSocketName());

		// Attaching armor pieces
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor))
		{
			int Counter = 0;
			for (const FName& SocketName : ArmorEquipped->GetArmorPiecesAttachmentSocketNames())
			{
				if (UStaticMeshComponent* MeshComponent = ArmorEquipped->GetArmorPieceMeshComponent_AtIndex(Counter))
				{
					MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					MeshComponent->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentTransformRules, SocketName);
					MeshComponent->SetVisibility(true);
				}
				Counter++;
			}
			return true;
		}
	}
	return false;
}

void UAttributeComponent::BreakArmor()
{
	if (ArmorEquipped)
	{
		ArmorEquipped->Break();
		ArmorEquipped = nullptr;
		
		OnArmorUpdated.Broadcast(0, 0, false);
	}
}


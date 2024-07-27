// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/CombatComponent.h"
#include "Characters/ShooterCharacter.h"
#include "Weapons/WeaponBase.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Weapons.Empty();
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::Init_Component()
{
	OwnerAsShooterCharacter = Cast<AShooterCharacter>(GetOwner());
}

bool UCombatComponent::CanTakeWeapon(const AWeaponBase* InWeapon) const
{
	if (!InWeapon) return false;
	for (auto& Weapon : Weapons)
		if (InWeapon->GetWeaponClass() == Weapon.Get<0>()) return false;
	return true;
}

bool UCombatComponent::AddWeapon(AWeaponBase* InWeapon)
{
	if (CanTakeWeapon(InWeapon) == false) return false;
	
	// Destroy previous weapon
	if (WeaponInHand)
	{
		OnWeaponDestroyBegin(WeaponInHand);
		WeaponInHand->Destroy();
	}

	// Add weapon class to list
	Weapons.Add(InWeapon->GetWeaponClass(), InWeapon->GetTotalAmmoAvailable());

	// Update ownership
	InWeapon->SetOwner(OwnerAsShooterCharacter);
	
	// Attach weapon to hand
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	InWeapon->AttachToComponent(OwnerAsShooterCharacter->GetMesh(),
		AttachmentTransformRules, InWeapon->GetWeaponProperties().InHandAttachmentSocketName);
	OwnerAsShooterCharacter->GetShooterProperties()->bEnableIK = InWeapon->GetWeaponProperties().bShouldEnableIK;
	
	InWeapon->OnWeaponPicked.Broadcast();
	
	WeaponInHand = InWeapon;
	WeaponIndex = Weapons.Num() - 1;
	OnWeaponSelected();
	OnWeaponUpdated.Broadcast();
	return true;
}

bool UCombatComponent::SelectWeapon(const int Index)
{
	if (!OwnerAsShooterCharacter || !GetWorld()) return false;
	if (Index < 0 || Weapons.Num() - 1 < Index) return false;
	
	// Destroy current weapon
	if (WeaponInHand)
	{
		OnWeaponDestroyBegin(WeaponInHand);
		WeaponInHand->Destroy();
	}

	TArray<TSubclassOf<AWeaponBase>> Keys;
	Weapons.GetKeys(Keys);
	const TSubclassOf<AWeaponBase> WeaponClassSelection = Keys[Index];
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = OwnerAsShooterCharacter;
	ActorSpawnParameters.Owner = OwnerAsShooterCharacter;
	AWeaponBase* WeaponSelection = GetWorld()->SpawnActor<AWeaponBase>(WeaponClassSelection, ActorSpawnParameters);
	OwnerAsShooterCharacter->GetShooterProperties()->bEnableIK = WeaponSelection->GetWeaponProperties().bShouldEnableIK;
	
	if (!WeaponSelection) return false;

	// Update ownership and broadcast OnWeaponSelected
	WeaponSelection->SetOwner(OwnerAsShooterCharacter);
	WeaponSelection->OnWeaponSelected.Broadcast();

	// Update ammo
	TArray<int> Values;
	Weapons.GenerateValueArray(Values);
	const int Ammo = Values[Index];
	WeaponSelection->SetTotalAmmoAvailable(Ammo);
	
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
	EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	WeaponSelection->AttachToComponent(OwnerAsShooterCharacter->GetMesh(),
		AttachmentTransformRules, WeaponSelection->GetWeaponProperties().InHandAttachmentSocketName);
	WeaponInHand = WeaponSelection;
	WeaponIndex = Index;
	OnWeaponSelected();
	OnWeaponUpdated.Broadcast();
	
	return true;
}

void UCombatComponent::FireButtonPressed()
{
	if (!WeaponInHand) return;
	WeaponInHand->OnWeaponFirePressed.Broadcast();
}

void UCombatComponent::OnWeaponSelected()
{
	WeaponInHand->OnWeaponFired.AddDynamic(this, &UCombatComponent::OnWeaponFiredCallback);
}

void UCombatComponent::OnWeaponDestroyBegin(AWeaponBase* InWeapon)
{
	InWeapon->OnWeaponFired.RemoveDynamic(this, &UCombatComponent::OnWeaponFiredCallback);
}

void UCombatComponent::OnWeaponFiredCallback()
{
	UpdateAmmoForCurrentWeapon();
}

void UCombatComponent::UpdateAmmoForCurrentWeapon()
{
	// Update ammo for current weapon
	int Local_Index = 0;
	for (auto& Weapon : Weapons)
	{
		if (Local_Index == WeaponIndex)
		{
			Weapon.Value = WeaponInHand->GetTotalAmmoAvailable();
			OnWeaponUpdated.Broadcast();
		}
		Local_Index++;
	}
}

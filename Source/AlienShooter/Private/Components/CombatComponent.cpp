// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/CombatComponent.h"
#include "Characters/ShooterCharacter.h"
#include "Weapons/WeaponBase.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Weapons.SetNum(0);
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
	for (auto& WeaponClass : Weapons)
		if (InWeapon->GetWeaponClass() == WeaponClass) return false;
	return true;
}

bool UCombatComponent::AddWeapon(AWeaponBase* InWeapon)
{
	if (CanTakeWeapon(InWeapon) == false) return false;
	
	// Destroy previous weapon
	if (WeaponInHand) WeaponInHand->Destroy();

	// Add weapon class to list
	Weapons.Add(InWeapon->GetWeaponClass());

	// Update ownership
	InWeapon->SetOwner(OwnerAsShooterCharacter);
	
	// Attach weapon to hand
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	InWeapon->AttachToComponent(OwnerAsShooterCharacter->GetMesh(),
		AttachmentTransformRules, InWeapon->GetWeaponProperties().InHandAttachmentSocketName);

	InWeapon->OnWeaponPicked.Broadcast();
	
	WeaponInHand = InWeapon;
	WeaponIndex = Weapons.Num() - 1;
	return true;
}

bool UCombatComponent::SelectWeapon(const int Index)
{
	if (!OwnerAsShooterCharacter || !GetWorld()) return false;
	if (Index < 0 || Weapons.Num() - 1 < Index) return false;

	// Destroy current weapon
	if (WeaponInHand) WeaponInHand->Destroy();
	
	const TSubclassOf<AWeaponBase> WeaponClassSelection = Weapons[Index];
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = OwnerAsShooterCharacter;
	ActorSpawnParameters.Owner = OwnerAsShooterCharacter;
	AWeaponBase* WeaponSelection = GetWorld()->SpawnActor<AWeaponBase>(WeaponClassSelection, ActorSpawnParameters);
	
	if (!WeaponSelection) return false;

	// Update ownership
	WeaponSelection->SetOwner(OwnerAsShooterCharacter);
	WeaponSelection->OnWeaponPicked.Broadcast();
	
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
	EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	WeaponSelection->AttachToComponent(OwnerAsShooterCharacter->GetMesh(),
		AttachmentTransformRules, WeaponSelection->GetWeaponProperties().InHandAttachmentSocketName);
	WeaponInHand = WeaponSelection;
	WeaponIndex = Index;
	
	return true;
}

void UCombatComponent::FireButtonPressed()
{
	if (!WeaponInHand) return;
	WeaponInHand->OnWeaponFired.Broadcast();
}

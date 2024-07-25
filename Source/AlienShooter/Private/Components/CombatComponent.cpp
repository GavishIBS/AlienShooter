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
	// TODO: Check if already have same weapon
	return true;
}

bool UCombatComponent::AddWeapon(AWeaponBase* InWeapon)
{
	if (CanTakeWeapon(InWeapon) == false) return false;
	
	// Destroy previous weapon
	if (WeaponInHand) WeaponInHand->Destroy();
	
	Weapons.Add(InWeapon->GetWeaponClass());

	// Attach weapon to hand
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	InWeapon->AttachToComponent(OwnerAsShooterCharacter->GetMesh(),
		AttachmentTransformRules, InWeapon->GetWeaponProperties().InHandAttachmentSocketName);

	InWeapon->OnWeaponPicked.Broadcast();
	
	WeaponInHand = InWeapon;
	return true;
}

bool UCombatComponent::SelectWeapon(const int Index)
{
	if (!OwnerAsShooterCharacter || !GetWorld()) return false;
	if (Weapons.Num() - 1 < Index) return false;
	
	const TSubclassOf<AWeaponBase> WeaponClassSelection = Weapons[Index];
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = OwnerAsShooterCharacter;
	ActorSpawnParameters.Owner = OwnerAsShooterCharacter;
	AWeaponBase* WeaponSelection = GetWorld()->SpawnActor<AWeaponBase>(WeaponClassSelection, ActorSpawnParameters);
	
	if (!WeaponSelection) return false;

	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
	EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	WeaponSelection->AttachToComponent(OwnerAsShooterCharacter->GetMesh(),
		AttachmentTransformRules, WeaponSelection->GetWeaponProperties().InHandAttachmentSocketName);
	WeaponInHand = WeaponSelection;

	return true;
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/CombatComponent.h"
#include "Characters/ShooterCharacter.h"
#include "Weapons/WeaponBase.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Weapons.Empty();
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (OwnerAsShooterCharacter && WeaponInHand && WeaponInHand->EnableLaser())
	{
		WeaponInHand->UpdateLaser(OwnerAsShooterCharacter->GetCursorTraceHitLocation(), OwnerAsShooterCharacter->IsCursorOverlappingHittable());
	}
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
		AttachmentTransformRules, InWeapon->GetWeaponProperties()->InHandAttachmentSocketName);
	
	InWeapon->OnWeaponPicked.Broadcast();
	
	WeaponInHand = InWeapon;
	WeaponIndex = Weapons.Num() - 1;
	OnWeaponSelected();
	OnNewWeaponAdded.Broadcast(WeaponInHand);
	OnWeaponUpdated.Broadcast();
	return true;
}

bool UCombatComponent::SelectWeapon(const int Index)
{
	if (!OwnerAsShooterCharacter || !GetWorld()) return false;
	if (Index < 0 || Weapons.Num() - 1 < Index) return false;

	// Destroy current weapon
	DestroyCurrentWeapon();

	TArray<TSubclassOf<AWeaponBase>> Keys;
	Weapons.GetKeys(Keys);
	const TSubclassOf<AWeaponBase> WeaponClassSelection = Keys[Index];
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = OwnerAsShooterCharacter;
	ActorSpawnParameters.Owner = OwnerAsShooterCharacter;
	AWeaponBase* WeaponSelection = GetWorld()->SpawnActor<AWeaponBase>(WeaponClassSelection, ActorSpawnParameters);
	
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
		AttachmentTransformRules, WeaponSelection->GetWeaponProperties()->InHandAttachmentSocketName);
	WeaponInHand = WeaponSelection;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("WeaponInHandSet: %s"), *WeaponInHand->GetName()));
	}

	WeaponIndex = Index;
	OnWeaponSelected();
	OnWeaponUpdated.Broadcast();
	
	return true;
}

bool UCombatComponent::UnselectWeapon()
{
	DestroyCurrentWeapon();
	return true;
}

void UCombatComponent::FireButtonPressed()
{
	if (!WeaponInHand) return;
	WeaponInHand->OnWeaponFirePressed.Broadcast();
	if (WeaponInHand->HaveAutomaticFire())
		StartAutomaticFire();
}

void UCombatComponent::FireButtonReleased()
{
	StopAutomaticFire();
}

void UCombatComponent::StartAutomaticFire()
{
	if (!WeaponInHand) return;

	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->GetWorldTimerManager().SetTimer(AutomaticFireTimerHandle, this, &UCombatComponent::FireButtonPressed, WeaponInHand->GetAutomaticFiringRate(), true, WeaponInHand->GetAutomaticFiringRate());
	}
}

void UCombatComponent::StopAutomaticFire()
{
	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->GetWorldTimerManager().ClearTimer(AutomaticFireTimerHandle);
	}
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

bool UCombatComponent::CanTakeAmmo(const TSubclassOf<AWeaponBase>& FitWeaponClass)
{
	if (!FitWeaponClass) return false;

	if (WeaponInHand && FitWeaponClass == WeaponInHand->GetClass())
	{
		if (*Weapons.Find(FitWeaponClass) >= WeaponInHand->GetMaxAmmoCapacity())
		{
			return false;
		}
		else {
			return true;
		}
	}

	for (auto& Weapon : Weapons)
	{
		if (FitWeaponClass == Weapon.Get<0>())
		{
			AWeaponBase* WeaponInstance = GetWorld()->SpawnActor<AWeaponBase>(FitWeaponClass);
			if (WeaponInstance)
			{
				if (Weapon.Value >= WeaponInstance->GetMaxAmmoCapacity())
				{
					WeaponInstance->Destroy();
					return false;
				}
				else { WeaponInstance->Destroy(); }
			}
			return true;
		}
	}
	return false;
}

bool UCombatComponent::TakeAmmo(const TSubclassOf<AWeaponBase>& FitWeaponClass, const int& AmmoValue)
{
	if (!CanTakeAmmo(FitWeaponClass)) return false;

	if (WeaponInHand && FitWeaponClass == WeaponInHand->GetClass())
	{
		WeaponInHand->SetTotalAmmoAvailable(WeaponInHand->GetTotalAmmoAvailable() + AmmoValue);
		UpdateAmmoForCurrentWeapon();
		return true;
	}

	for (auto& Weapon : Weapons)
	{
		if (FitWeaponClass == Weapon.Get<0>())
		{
			Weapon.Value += AmmoValue;
			OnWeaponUpdated.Broadcast();
			return true;
		}
	}

	return false;
}

void UCombatComponent::DestroyCurrentWeapon()
{
	if (WeaponInHand)
	{
		OnWeaponDestroyBegin(WeaponInHand);
		WeaponInHand->Destroy();
		WeaponInHand = nullptr;
	}
}

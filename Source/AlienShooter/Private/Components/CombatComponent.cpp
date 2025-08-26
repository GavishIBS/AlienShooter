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

bool UCombatComponent::AddWeaponFromClass(TSubclassOf<AActor> InWeaponClass)
{
	if (InWeaponClass == nullptr) return false;

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = OwnerAsShooterCharacter;
	ActorSpawnParameters.Owner = OwnerAsShooterCharacter;
	AWeaponBase* WeaponSelection = GetWorld()->SpawnActor<AWeaponBase>(InWeaponClass, ActorSpawnParameters);
	return AddWeapon(WeaponSelection);
}

bool UCombatComponent::AddWeapon(AWeaponBase* InWeapon)
{
	if (CanTakeWeapon(InWeapon) == false) return false;
	
	// Destroy current weapon
	DestroyCurrentWeapon();

	// Add new weapon class to list
	Weapons.Add(InWeapon->GetWeaponClass(), InWeapon->GetTotalAmmoAvailable());

	// Update ownership
	InWeapon->SetOwner(OwnerAsShooterCharacter);
	
	// Attach new weapon to hand
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	InWeapon->AttachToComponent(OwnerAsShooterCharacter->GetMesh(),
		AttachmentTransformRules, InWeapon->GetWeaponProperties()->InHandAttachmentSocketName);
	
	InWeapon->OnMonsterKill.AddDynamic(this, &UCombatComponent::OnMonsterKilled);

	InWeapon->OnWeaponPicked.Broadcast();
	InWeapon->OnWeaponSelected.Broadcast();
	
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

	// Update ownership
	WeaponSelection->SetOwner(OwnerAsShooterCharacter);

	// Update ammo
	TArray<int> Values;
	Weapons.GenerateValueArray(Values);
	int Ammo = FMath::Clamp(Values[Index], 0, WeaponSelection->GetWeaponProperties()->MaxAmmoCapacity + WeaponSelection->GetWeaponProperties()->MagazineMaxSize);
	if (Ammo >= WeaponSelection->GetWeaponProperties()->MagazineMaxSize) {
		WeaponSelection->SetAmmoInMagazine(WeaponSelection->GetWeaponProperties()->MagazineMaxSize);
	}
	else {
		WeaponSelection->SetAmmoInMagazine(Ammo);
	}
	int RemainingAmmo = FMath::Clamp(Ammo - WeaponSelection->GetAmmoInMag(), 0, WeaponSelection->GetWeaponProperties()->MaxAmmoCapacity);
	WeaponSelection->SetTotalAmmoAvailable(RemainingAmmo);
	
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
	EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	WeaponSelection->AttachToComponent(OwnerAsShooterCharacter->GetMesh(),
		AttachmentTransformRules, WeaponSelection->GetWeaponProperties()->InHandAttachmentSocketName);
	WeaponInHand = WeaponSelection;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("WeaponInHandSet: %s"), *WeaponInHand->GetName()));
	}

	// Broadcast OnWeaponSelected
	WeaponSelection->OnWeaponSelected.Broadcast();

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

void UCombatComponent::StartAiming()
{
	GetWorld()->GetTimerManager().ClearTimer(EndAimTimer);
	bAiming = true;
}

void UCombatComponent::EndAiming()
{
	bAiming = false;
}

void UCombatComponent::EndAimingWithDelay()
{
	GetWorld()->GetTimerManager().SetTimer(EndAimTimer, this, &ThisClass::EndAiming, EndAimDelay, false, EndAimDelay);
}

void UCombatComponent::FireButtonPressed()
{
	if (WeaponInHand->HaveAutomaticFire())
		StartAutomaticFire();
	else FireWeapon();
}

bool UCombatComponent::CanFire()
{
	return OwnerAsShooterCharacter ? OwnerAsShooterCharacter->CanFire() : false;
}

void UCombatComponent::FireWeapon()
{
	//if (WeaponInHand == nullptr) return;
	if (CanFire() == false) return;

	if (WeaponInHand->CanAim() && WeaponInHand->ShouldForceAutomaticAim() && bAiming == false) {
		StartAiming();
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("AmmoInMagazine: %d\nMagazineMaxCapacity: %d\nTotalAmmoAvailable: %d\nAmmoInBag: %d"), 0.f, 0.f, WeaponInHand->GetTotalAmmoAvailable(), WeaponInHand->GetAmmoInBag()));
	
	if (WeaponInHand->IsMagazineEmpty()) {
		if (WeaponInHand->TryReload()) {
			OnWeaponUpdated.Broadcast();
		}
		StopAutomaticFire();
		return;
	}
	if (WeaponInHand->ShouldDoCameraShake()) {
		//OwnerAsShooterCharacter->DoCameraShake(WeaponInHand->GetCameraShakeIntensity(), WeaponInHand->GetCameraShakeTime(), true, OwnerAsShooterCharacter->GetMesh()->GetForwardVector());
		OwnerAsShooterCharacter->DoCameraShake(WeaponInHand->GetCameraShakeIntensity(), WeaponInHand->GetCameraShakeTime(), false, FVector());
	}
	WeaponInHand->OnWeaponFirePressed.Broadcast();
}

void UCombatComponent::FireButtonReleased()
{
	if (WeaponInHand->CanAim() && WeaponInHand->ShouldForceAutomaticAim()) {
		EndAimingWithDelay();
	}
	StopAutomaticFire();
}

void UCombatComponent::StartAutomaticFire()
{
	if (!WeaponInHand) return;

	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->GetWorldTimerManager().SetTimer(AutomaticFireTimerHandle, this, &UCombatComponent::FireWeapon, WeaponInHand->GetAutomaticFiringRate(), true, 0.f);
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
		WeaponInHand->OnMonsterKill.Clear();
		StopAutomaticFire();
		OnWeaponDestroyBegin(WeaponInHand);
		WeaponInHand->Destroy();
		WeaponInHand = nullptr;
	}
}

void UCombatComponent::OnMonsterKilled()
{
	if (OwnerAsShooterCharacter)
	{
		OwnerAsShooterCharacter->TriggerCombatSelfDialogueWrapper();
	}
}

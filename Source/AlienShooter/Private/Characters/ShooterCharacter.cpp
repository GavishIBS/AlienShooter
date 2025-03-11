// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Characters/ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/CombatComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/PlayerController.h"
#include "HUD/TDSHUD.h"
#include "UI/Widgets/TDS_InventoryMenu.h"
#include "AnimationLibrary.h"
#include "Components/SpecialAbilityComponent.h"
#include "NiagaraComponent.h"
#include "Actors/UsableActor.h"
#include "Components/RadioPhoneComponent.h"
#include "UI/Widgets/TDSOverlay.h"

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpecialAbilityNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpecialAbilityNiagaraComponent"));
	SpecialAbilityNiagaraComponent->SetupAttachment(GetMesh());
	SpecialAbilityNiagaraComponent->Deactivate();
	SpecialAbilityNiagaraComponent->bAutoActivate = false;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	SpecialAbilityComponent = CreateDefaultSubobject<USpecialAbilityComponent>(TEXT("SpecialAbilityComponent"));
}

void AShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent) CombatComponent->Init_Component();
	if (SpecialAbilityComponent) SpecialAbilityComponent->Init_Component();
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitCallbacks();
}

void AShooterCharacter::InitCallbacks()
{
	OnTakeAnyDamage.AddDynamic(this, &AShooterCharacter::OnTakeAnyDamageCallback);
	OnDeath.AddDynamic(this, &AShooterCharacter::OnDeathCallback);
	if (CombatComponent) {
		CombatComponent->OnWeaponUpdated.AddDynamic(this, &AShooterCharacter::OnWeaponInHandUpdated);
	}
	if (RadioPhoneComponent) {
		RadioPhoneComponent->OnAudioTriggered.AddDynamic(this, &AShooterCharacter::OnRadioPhoneAudioTriggered);
	}
}

void AShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LeftMouseButtonAction, ETriggerEvent::Started, this, &AShooterCharacter::LeftMouseButtonPressed);
		EnhancedInputComponent->BindAction(LeftMouseButtonAction, ETriggerEvent::Completed, this, &AShooterCharacter::LeftMouseButtonReleased);
		
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Started, this, &AShooterCharacter::RightMouseButtonPressed);
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Completed, this, &AShooterCharacter::RightMouseButtonReleased);

		EnhancedInputComponent->BindAction(MouseWheelUpAction, ETriggerEvent::Started, this, &AShooterCharacter::MouseWheelUp);
		EnhancedInputComponent->BindAction(MouseWheelDownAction, ETriggerEvent::Started, this, &AShooterCharacter::MouseWheelDown);
		
		EnhancedInputComponent->BindAction(LeftAltAction, ETriggerEvent::Started, this, &AShooterCharacter::LeftAltPressed);
		EnhancedInputComponent->BindAction(LeftAltAction, ETriggerEvent::Completed, this, &AShooterCharacter::LeftAltReleased);
		
		EnhancedInputComponent->BindAction(SpecialAbilityAction, ETriggerEvent::Started, this, &AShooterCharacter::AbilityKeyPressed);

		EnhancedInputComponent->BindAction(InventorySlot_1_Action, ETriggerEvent::Started, this, &AShooterCharacter::InventorySlotPressed_1);
		EnhancedInputComponent->BindAction(InventorySlot_2_Action, ETriggerEvent::Started, this, &AShooterCharacter::InventorySlotPressed_2);
		EnhancedInputComponent->BindAction(InventorySlot_3_Action, ETriggerEvent::Started, this, &AShooterCharacter::InventorySlotPressed_3);
		EnhancedInputComponent->BindAction(InventorySlot_4_Action, ETriggerEvent::Started, this, &AShooterCharacter::InventorySlotPressed_4);
	}
}

float AShooterCharacter::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	if (LifeStatus != ELifeStatus::ELS_Alive) return 0.f;

	const float Damage_Calc = Damage +
		UKismetMathLibrary::RandomFloatInRange(-RandomizationFactor, RandomizationFactor);
	const FDamageEvent DamageEvent;
	TakeDamage(Damage_Calc, DamageEvent, nullptr, InDamageCauser);
	return Damage_Calc;
}

void AShooterCharacter::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!AttributeComponent) return;
	if (LifeStatus != ELifeStatus::ELS_Alive) return;

	AttributeComponent->TakeDamage(Damage);
	
	if (AttributeComponent->GetHealth() <= 0.f)
	{
		// Death
		OnDeath.Broadcast(DamageCauser);
	}
}

void AShooterCharacter::OnDeathCallback(AActor* Killer)
{
	if (!PlayerController) return;
	PlayerController->DisableInput(PlayerController);
	LifeStatus = ELifeStatus::ELS_Dead;
}

void AShooterCharacter::LeftMouseButtonPressed()
{
	//if (CombatComponent && CombatComponent->WeaponInHand && PawnActionState < EPawnActionState::EPAS_Attacking)
	if (CanFire())
	{
		CombatComponent->FireButtonPressed();
		//if (AnimationLibrary::PlayAnimationMontage(GetMesh(), CombatComponent->WeaponInHand->GetWeaponAnimationData_Pawn()->FireAnimationMontages_Pawn))
			//PawnActionState = EPawnActionState::EPAS_Attacking;
	}
	else if (InventoryComponent && InventoryComponent->ItemInHand)
	{
		if (AUsableActor* UseItem = InventoryComponent->UseButtonPressed()) {
			AnimationLibrary::PlayAnimationMontage(GetMesh(), {UseItem->GetItemUseAnimMontage()});
			if (CombatComponent) {
				CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
			}
		}
	}
}

void AShooterCharacter::LeftMouseButtonReleased()
{
	if (CombatComponent && CombatComponent->WeaponInHand && PawnActionState <= EPawnActionState::EPAS_Attacking)
	{
		CombatComponent->FireButtonReleased();
	}
}

void AShooterCharacter::RightMouseButtonPressed()
{
	if (CombatComponent && CombatComponent->WeaponInHand)
		CombatComponent->StartAiming();
}

void AShooterCharacter::RightMouseButtonReleased()
{
	if (CombatComponent && CombatComponent->WeaponInHand)
		CombatComponent->EndAimingWithDelay();
}

void AShooterCharacter::MouseWheelUp()
{
	if (!CombatComponent) return;

	if (PawnActionState == EPawnActionState::EPAS_SelectingItem) {
		if (!InventoryComponent) return;
		if (InventoryComponent->SelectItem(InventoryComponent->SelectedItemIndex + 1)) {
			CombatComponent->UnselectWeapon();
		}
		return;
	}

	if (PawnActionState >= EPawnActionState::EPAS_Attacking) return;

	InventoryComponent->UnselectItem();
	if (CombatComponent->WeaponInHand)
		CombatComponent->SelectWeapon(CombatComponent->WeaponIndex + 1);
	else CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
}

/*
void AShooterCharacter::MouseWheelUp()
{
	if (PawnActionState >= EPawnActionState::EPAS_Attacking) return;
	if (!CombatComponent) return;

	bool bLeftAltDown = PlayerController->IsInputKeyDown(EKeys::LeftAlt);

	if (!bLeftAltDown)
	{
		InventoryComponent->UnselectItem();
		if (CombatComponent->WeaponInHand)
			CombatComponent->SelectWeapon(CombatComponent->WeaponIndex + 1);
		else CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
	}
	else {
		if (!InventoryComponent) return;
		if (InventoryComponent->SelectItem(InventoryComponent->SelectedItemIndex + 1))
			CombatComponent->UnselectWeapon();
	}
}
*/

void AShooterCharacter::MouseWheelDown()
{
	if (!CombatComponent) return;
	
	if (PawnActionState == EPawnActionState::EPAS_SelectingItem) {
		if (!InventoryComponent) return;
		if (InventoryComponent->SelectItem(InventoryComponent->SelectedItemIndex - 1))
			CombatComponent->UnselectWeapon();
		return;
	}

	if (PawnActionState >= EPawnActionState::EPAS_Attacking) return;

	InventoryComponent->UnselectItem();
	if (CombatComponent->WeaponInHand)
		CombatComponent->SelectWeapon(CombatComponent->WeaponIndex - 1);
	else CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
}

/*
void AShooterCharacter::MouseWheelDown()
{
	if (PawnActionState >= EPawnActionState::EPAS_Attacking) return;
	if (!CombatComponent) return;

	bool bLeftAltDown = PlayerController->IsInputKeyDown(EKeys::LeftAlt);

	if (!bLeftAltDown)
	{
		InventoryComponent->UnselectItem();
		if (CombatComponent->WeaponInHand)
			CombatComponent->SelectWeapon(CombatComponent->WeaponIndex - 1);
		else CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
	}
	else {
		if (!InventoryComponent) return;
		if (InventoryComponent->SelectItem(InventoryComponent->SelectedItemIndex - 1))
			CombatComponent->UnselectWeapon();
	}
}
*/

void AShooterCharacter::LeftAltPressed()
{
	if (PawnActionState >= EPawnActionState::EPAS_Attacking) return;
	OpenInventory_UI();
	PawnActionState = EPawnActionState::EPAS_SelectingItem;
}

void AShooterCharacter::LeftAltReleased()
{
	if (PawnActionState >= EPawnActionState::EPAS_Attacking &&
		PawnActionState != EPawnActionState::EPAS_SelectingItem) return;
	CloseInventory_UI();
	PawnActionState = EPawnActionState::EPAS_Idle;
}

void AShooterCharacter::AbilityKeyPressed()
{
	if (SpecialAbilityComponent) {
		SpecialAbilityComponent->UseAbility();
	}
}

void AShooterCharacter::InventorySlotPressed_1()
{
	TryUseInventoryItemAtIndex(0);
}

void AShooterCharacter::InventorySlotPressed_2()
{
	TryUseInventoryItemAtIndex(1);
}

void AShooterCharacter::InventorySlotPressed_3()
{
	TryUseInventoryItemAtIndex(2);
}

void AShooterCharacter::InventorySlotPressed_4()
{
	TryUseInventoryItemAtIndex(3);
}

void AShooterCharacter::EndAttack()
{
	if (PawnActionState > EPawnActionState::EPAS_Attacking) return;
	PawnActionState = EPawnActionState::EPAS_Idle;
}

void AShooterCharacter::EndReload()
{
	if (PawnActionState > EPawnActionState::EPAS_Reloading) return;
	PawnActionState = EPawnActionState::EPAS_Idle;
}

void AShooterCharacter::OnInventoryItemSuccessfullyUsed()
{
	if (InventoryComponent) {
		InventoryComponent->OnItemUsed();
	}
	if (CombatComponent != nullptr && CombatComponent->WeaponInHand == nullptr) {
		CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
	}
}

void AShooterCharacter::OnWeaponInHandUpdated()
{
	if (CombatComponent && CombatComponent->WeaponInHand) {
		CombatComponent->WeaponInHand->OnWeaponFired.AddDynamic(this, &AShooterCharacter::OnWeaponFired);
		CombatComponent->WeaponInHand->OnWeaponReload.AddDynamic(this, &AShooterCharacter::OnWeaponReload);
	}
}

void AShooterCharacter::OnWeaponFired()
{
	if (CombatComponent == nullptr || CombatComponent->WeaponInHand == nullptr) return;

	if (AnimationLibrary::PlayAnimationMontage(GetMesh(), CombatComponent->WeaponInHand->GetWeaponAnimationData_Pawn()->FireAnimationMontages_Pawn))
		PawnActionState = EPawnActionState::EPAS_Attacking;
}

void AShooterCharacter::OnWeaponReload()
{
	if (CombatComponent == nullptr || CombatComponent->WeaponInHand == nullptr) return;

	if (AnimationLibrary::PlayAnimationMontage(GetMesh(), CombatComponent->WeaponInHand->GetWeaponAnimationData_Pawn()->ReloadAnimationMontages_Pawn))
		PawnActionState = EPawnActionState::EPAS_Reloading;
}

void AShooterCharacter::OpenInventory_UI()
{
	if (!PlayerController) return;

	if (ATDSHUD* TDS_HUD = Cast<ATDSHUD>(PlayerController->GetHUD()))
	{
		InventoryComponent->SetInventory_UW(Cast<UTDS_InventoryMenu>(TDS_HUD->MakeWidgetInViewport(InventoryComponent->InventoryMenu_UW_Class)));
	}
}

void AShooterCharacter::CloseInventory_UI()
{
	InventoryComponent->CloseInventory_UI();
}

bool AShooterCharacter::CanFire()
{
	if (CombatComponent && CombatComponent->WeaponInHand && PawnActionState < EPawnActionState::EPAS_Attacking)
		return true;
	else return false;
}

void AShooterCharacter::PickupWeapon(AWeaponBase* InWeapon) const
{
	if (!CombatComponent) return;
	CombatComponent->AddWeapon(InWeapon);
}

void AShooterCharacter::TryUseInventoryItemAtIndex(const int Index)
{
	if (!InventoryComponent || InventoryComponent->InventorySize < (Index + 1)) return;

	if (InventoryComponent->IsItemUsableOnIndex(Index)) {
		if (CombatComponent)
			CombatComponent->UnselectWeapon();
		InventoryComponent->SelectItem(Index);

		//bool bUsed = false;
		if (AUsableActor* UseItem = InventoryComponent->UseButtonPressed()) {
			AnimationLibrary::PlayAnimationMontage(GetMesh(), { UseItem->GetItemUseAnimMontage() });
			//bUsed = true;
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString("Failed to use item"));
			InventoryComponent->UnselectItem();
			if (CombatComponent)
				CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
		}

		/*
		if (bUsed == false) {
			InventoryComponent->UnselectItem();
			if (CombatComponent)
				CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
		}
		*/
	}
}

void AShooterCharacter::DeactivateSpecialAbilityNiagaraEffect()
{
	SpecialAbilityNiagaraComponent->Deactivate();
}

void AShooterCharacter::ActivateSpecialAbilityNiagaraEffect(UNiagaraSystem* InSystem) {
	SpecialAbilityNiagaraComponent->Activate();
	SpecialAbilityNiagaraComponent->SetAsset(InSystem);
}

void AShooterCharacter::OnRadioPhoneAudioTriggered(const EAudioType AudioType, const FString AudioInfo, const float AudioDuration) {
	if (AudioType != EAudioType::EAT_Dialogue) return;
	if (!PlayerController) return;

	if (ATDSHUD* TDS_HUD = Cast<ATDSHUD>(PlayerController->GetHUD()))  // TODO: Save TDS_HUD at BeginPlay
	{
		TDS_HUD->GetOverlayWidget()->SetEnvironmentDialogue_Subtitles_Text(FText::FromString(AudioInfo), AudioDuration);
	}
}

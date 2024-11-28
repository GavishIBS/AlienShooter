// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Characters/ShooterCharacter_MeleeAttacker.h"
#include "EnhancedInputComponent.h"
#include "Components/CombatComponent.h"
#include "Components/InventoryComponent.h"
#include "Weapons/MeleeWeaponBase.h"

AShooterCharacter_MeleeAttacker::AShooterCharacter_MeleeAttacker()
{
}

void AShooterCharacter_MeleeAttacker::BeginPlay()
{
	Super::BeginPlay();
}

bool AShooterCharacter_MeleeAttacker::CanAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("ActionState: %d"), PawnActionState));
	return PawnActionState >= EPawnActionState::EPAS_Attacking ? false : true;
}

void AShooterCharacter_MeleeAttacker::MeleeAttackKeyDown()
{
	if (!CanAttack()) return;
	UWorld* World = GetWorld();
	if (!World) return;

	// TODO: Disable all other input expect this action

	checkf(MeleeWeaponClass, TEXT("MeleeWeaponClass is not set in melee weapon BP"));

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.Owner = this;
	ActorSpawnParams.Instigator = this;
	MeleeWeapon = World->SpawnActor<AMeleeWeaponBase>(MeleeWeaponClass, FTransform(), ActorSpawnParams);
	if (!MeleeWeapon) return;

	CombatComponent->UnselectWeapon();
	InventoryComponent->UnselectItem();

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld, false);
	MeleeWeapon->AttachToComponent(GetMesh(), AttachmentTransformRules, MeleeWeapon->GetAttachmentSocketName());

	PawnActionState = EPawnActionState::EPAS_Attacking_Melee;
}

void AShooterCharacter_MeleeAttacker::MeleeAttackKeyUp()
{
	// TODO: Enable all the input actions again
	
	if (!MeleeWeapon || MeleeWeapon->Tags.Contains("Attacking")) return;

	if (UAnimMontage* AnimMontage = MeleeWeapon->GetAttackMontage(false)) {
		PlayAnimMontage(AnimMontage);
		MeleeWeapon->Tags.Add("Attacking");
	}
	else {
		MeleeWeapon->Destroy();
		CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
		PawnActionState = EPawnActionState::EPAS_Idle;
	}
}

void AShooterCharacter_MeleeAttacker::EndMeleeAttack()
{
	MeleeWeapon->Destroy();
	CombatComponent->SelectWeapon(CombatComponent->WeaponIndex);
	PawnActionState = EPawnActionState::EPAS_Idle;
}

void AShooterCharacter_MeleeAttacker::StartMeleeAttackTrace()
{
	if (MeleeWeapon)
		MeleeWeapon->StartAttackTrace();
}

void AShooterCharacter_MeleeAttacker::StopMeleeAttackTrace()
{
	if (MeleeWeapon)
		MeleeWeapon->StopAttackTrace();
}

void AShooterCharacter_MeleeAttacker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MeleeAttackAction, ETriggerEvent::Started, this, &AShooterCharacter_MeleeAttacker::MeleeAttackKeyDown);
		EnhancedInputComponent->BindAction(MeleeAttackAction, ETriggerEvent::Completed, this, &AShooterCharacter_MeleeAttacker::MeleeAttackKeyUp);
	}
}

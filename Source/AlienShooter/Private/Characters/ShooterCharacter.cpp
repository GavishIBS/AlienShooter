// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Characters/ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Components/CombatComponent.h"

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}

void AShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent) CombatComponent->Init_Component();
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
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
		EnhancedInputComponent->BindAction(LeftMouseButtonPressedAction, ETriggerEvent::Started, this, &AShooterCharacter::LeftMouseButtonPressed);
		EnhancedInputComponent->BindAction(MouseWheelUpAction, ETriggerEvent::Started, this, &AShooterCharacter::MouseWheelUp);
		EnhancedInputComponent->BindAction(MouseWheelDownAction, ETriggerEvent::Started, this, &AShooterCharacter::MouseWheelDown);
	}
}

void AShooterCharacter::LeftMouseButtonPressed()
{
	if (CombatComponent)
		CombatComponent->FireButtonPressed();
}

void AShooterCharacter::MouseWheelUp()
{
	if (!CombatComponent) return;
	CombatComponent->SelectWeapon(CombatComponent->WeaponIndex + 1);
}

void AShooterCharacter::MouseWheelDown()
{
	if (!CombatComponent) return;
	CombatComponent->SelectWeapon(CombatComponent->WeaponIndex - 1);
}

void AShooterCharacter::PickupWeapon(AWeaponBase* InWeapon) const
{
	if (!CombatComponent) return;
	CombatComponent->AddWeapon(InWeapon);
}

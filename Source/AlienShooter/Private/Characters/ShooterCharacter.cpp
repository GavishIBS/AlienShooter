// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Characters/ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/CombatComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"

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
	InitCallbacks();
}

void AShooterCharacter::InitCallbacks()
{
	OnTakeAnyDamage.AddDynamic(this, &AShooterCharacter::OnTakeAnyDamageCallback);
	OnDeath.AddDynamic(this, &AShooterCharacter::OnDeathCallback);
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

float AShooterCharacter::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser)
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

	float NewHealth = AttributeComponent->GetHealth() - Damage;
	NewHealth = FMath::Clamp(NewHealth, 0.f, AttributeComponent->GetMaxHealth());
	AttributeComponent->SetHealth(NewHealth);
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Cyan, FString::Printf(TEXT("NewHealth: %f"), NewHealth));

	if (NewHealth <= 0.f)
	{
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

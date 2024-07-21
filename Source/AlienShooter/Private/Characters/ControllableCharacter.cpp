// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Characters/ControllableCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

AControllableCharacter::AControllableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	CameraBoom->bUsePawnControlRotation = false;
	ViewCamera->bUsePawnControlRotation = false;
}

void AControllableCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 1);
		}
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}

void AControllableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCharacterRotationToCursor();
}

void AControllableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AControllableCharacter::Move);
	}
}

void AControllableCharacter::UpdateCharacterRotationToCursor() const
{
	if (!PlayerController) return;

	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	if (HitResult.bBlockingHit)
	{
		// Calculate new rotation
		const FVector HitLocation = HitResult.Location;
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetComponentLocation(), HitLocation);
		FRotator NewRotation = FRotator(GetMesh()->GetComponentRotation().Pitch, LookAtRotation.Yaw - 90.f, GetMesh()->GetComponentRotation().Roll);
		// NewRotation.Yaw is LookAtRotation.Yaw - 90.f and not LookAtRotation.Yaw because character mesh component is
		// rotated by -90.f yaw value relative to the capsule component.
		
		// Smooth rotation
		const FRotator CurrentRotation = GetMesh()->GetComponentRotation();
		NewRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), 10.f);

		// Update rotation
		GetMesh()->SetWorldRotation(NewRotation);
	}
}

void AControllableCharacter::Move(const FInputActionValue& Value)
{
	if (!GetController()) return;

	const FVector2d InputValue = Value.Get<FVector2d>();
	const FRotator YawRotation = FRotator(0.f, ViewCamera->GetComponentRotation().Yaw, 0.f);

	if (InputValue.X != 0)
	{
		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDir, InputValue.X);
	}

	if (InputValue.Y != 0)
	{
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(RightDir, InputValue.Y);
	}
}

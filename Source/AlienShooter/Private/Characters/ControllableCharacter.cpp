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
#include "Kismet/GameplayStatics.h"



#include "Components/AttributeComponent.h"

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
	if (AttributeComponent)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("CurrentHealth: %f"), AttributeComponent->GetHealth()));
	}
	if (LifeStatus != ELifeStatus::ELS_Dead) {
		CheckCursorLocation();
		UpdateCharacterRotationToCursor();
		UpdateCameraBoomOffset(DeltaTime);
	}
}

void AControllableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AControllableCharacter::Move);
	}
}

void AControllableCharacter::UpdateCharacterRotationToCursor()
{
	if (!PlayerController) return;

	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	if (HitResult.bBlockingHit)
	{
		// Calculate new rotation
		CursorTraceHitLocation = HitResult.Location;
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetComponentLocation(), CursorTraceHitLocation);
		FRotator NewRotation = FRotator(GetMesh()->GetComponentRotation().Pitch, LookAtRotation.Yaw - 90.f, GetMesh()->GetComponentRotation().Roll);
		// NewRotation.Yaw is LookAtRotation.Yaw - 90.f and not LookAtRotation.Yaw because character mesh component is
		// rotated by -90.f yaw value relative to the capsule component.
		
		// Smooth rotation
		const FRotator CurrentRotation = GetMesh()->GetComponentRotation();
		NewRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), 10.f);

		// Update rotation
		GetMesh()->SetWorldRotation(NewRotation);

		// Is cursor above a hittable actor
		//CursorOverlappingHittable = HitResult.GetActor()->Implements<IHitInterface>();
		if (Cast<IHitInterface>(HitResult.GetActor()))
		{
			CursorOverlappingHittable = true;
		}
		else CursorOverlappingHittable = false;
	}
	else {
		CursorOverlappingHittable = false;
	}
}

void AControllableCharacter::UpdateCameraBoomOffset(float DeltaTime)
{
	if (!PlayerController) return;

	FVector TargetOffset = FVector();

	FVector2D CursorPosition = FVector2D(CursorTraceHitLocation.X, CursorTraceHitLocation.Y);

	const float Camera_Cursor_Dist = FVector::Dist(FVector(CameraBoom->GetComponentLocation().X, CameraBoom->GetComponentLocation().Y, 0), FVector(CursorPosition.X, CursorPosition.Y, 0));
	if (FMath::Abs(Camera_Cursor_Dist - CameraResponse_InnerRadius) < CameraResponse_RadiusLease) return;  // avoid camera hinderence

	
	FVector CameraBoomTargetLocation = FVector();
	if (Camera_Cursor_Dist > CameraResponse_InnerRadius)
	{
		// Move the camera based on comparison between cursor position this frame and last frame
		const FVector TargetOffsetDirection = (CameraBoom->GetComponentLocation() - FVector(CursorPosition.X, CursorPosition.Y, 0.f)).GetSafeNormal();
		CameraBoomTargetLocation = CameraBoom->GetComponentLocation() + TargetOffsetDirection * CameraOffsetFactor;
	}
	/*
	else if (CurrentCameraBoomOffset.Size() > 20.f)  // && check is offset is in opposite dorection
	{
		// Move camera towards actor itself
		const FVector TargetOffsetDirection = (CameraBoom->GetComponentLocation() - GetActorLocation()).GetSafeNormal();
		CameraBoomTargetLocation = CameraBoom->GetComponentLocation() + TargetOffsetDirection * 1.f;
	}*/
	else {
		// Move camera towards actor itself
		const FVector TargetOffsetDirection = (CameraBoom->GetComponentLocation() - GetActorLocation()).GetSafeNormal();
		CameraBoomTargetLocation = CameraBoom->GetComponentLocation() + TargetOffsetDirection * 1.f;
	}

	CameraBoomTargetLocation.Z = CameraBoom->GetComponentLocation().Z;

	const FVector TargetCameraBoomOffset = CameraBoom->GetComponentLocation() - CameraBoomTargetLocation;
	CurrentCameraBoomOffset = FMath::VInterpTo(CameraBoomOffset_LastFrame, TargetCameraBoomOffset, DeltaTime, CameraSpeed);

	FVector NewLoc = CameraBoom->GetComponentLocation() + CurrentCameraBoomOffset - CameraBoomOffset_LastFrame;
	CameraBoom->SetWorldLocation(CameraBoom->GetComponentLocation() + CurrentCameraBoomOffset - CameraBoomOffset_LastFrame);

	CursorPosition_LastFrame = CursorPosition;
	CameraBoomOffset_LastFrame = CurrentCameraBoomOffset;
}

void AControllableCharacter::CheckCursorLocation()
{
	FVector2D CursorPosition;
	if (PlayerController->GetMousePosition(CursorPosition.X, CursorPosition.Y))
	{
		FVector CharacterWorldPosition = GetActorLocation();
		FVector2D CharacterScreenPosition;

		if (PlayerController->ProjectWorldLocationToScreen(CharacterWorldPosition, CharacterScreenPosition))
		{
			float Distance = FVector2D::Distance(CursorPosition, CharacterScreenPosition);
			if (Distance < RestrictedRadius)
			{
				FVector2D NewCursorPosition = CursorPosition + (CursorPosition - CharacterScreenPosition).GetSafeNormal() * (RestrictedRadius - Distance);
				PlayerController->SetMouseLocation(NewCursorPosition.X, NewCursorPosition.Y);
			}
		}
	}
}

void AControllableCharacter::Move(const FInputActionValue& Value)
{
	if (PawnActionState >= EPawnActionState::EPAS_Attacking_Melee) return;
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

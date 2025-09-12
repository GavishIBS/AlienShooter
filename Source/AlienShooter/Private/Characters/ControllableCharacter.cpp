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
#include "Components/SpotLightComponent.h"
#include "Game/Subsystems/OptionsGameInstanceSubsystem.h"

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

	FlashlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight_SpotLightComponent"));
	FlashlightComponent->SetupAttachment(GetMesh());
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

	// Update static variables
	BaseCameraBoomLength = CameraBoom->TargetArmLength;
	BaseCameraBoomRotationYaw = CameraBoom->GetRelativeRotation().Yaw;

	// Update camera settings
	UOptionsGameInstanceSubsystem* OptionsGIS = GetGameInstance()->GetSubsystem<UOptionsGameInstanceSubsystem>();
	if (OptionsGIS) {
		CameraSettings = OptionsGIS->CameraSettings;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("New camera settings applied"));
		/*
		if (CameraSettings.bEnableCameraBoomOffset) {
			UpdateCameraBoomOffset(0, true);
		}
		if (CameraSettings.bEnableCameraBoomRotation) {
			UpdateCameraBoomRotation(0, true);
		}
		*/
	}
}

void AControllableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AttributeComponent)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("CurrentHealth: %f"), AttributeComponent->GetHealth()));
	}

	if (LifeStatus != ELifeStatus::ELS_Dead && GetCharacterMovement()) {
		CheckCursorLocation();
		
		UpdateCharacterRotationToCursor();
		
		if (CameraSettings.bEnableCameraBoomOffset) {
			UpdateCameraBoomOffset(DeltaTime);
		}
		
		UpdateCameraBoomLength(DeltaTime);
		
		if (CameraSettings.bEnableCameraBoomOffset) {
			UpdateCameraBoomRotation(DeltaTime);
		}
	}
}

void AControllableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AControllableCharacter::Move);
		EnhancedInputComponent->BindAction(GamepadCursorMoveAction, ETriggerEvent::Triggered, this, &AControllableCharacter::GamepadCursorMove);
		EnhancedInputComponent->BindAction(ToggleFlashlightAction, ETriggerEvent::Completed, this, &AControllableCharacter::ToggleFlashlight);
		EnhancedInputComponent->BindAction(ToggleMovementTypeAction, ETriggerEvent::Completed, this, &AControllableCharacter::ToggleMovementType);
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
		const FRotator TargetRotation = FRotator(GetMesh()->GetComponentRotation().Pitch, LookAtRotation.Yaw - 90.f, GetMesh()->GetComponentRotation().Roll);
		// NewRotation.Yaw is LookAtRotation.Yaw - 90.f and not LookAtRotation.Yaw because character mesh component is
		// rotated by -90.f yaw value relative to the capsule component.
		
		// Smooth rotation
		const FRotator CurrentRotation = GetMesh()->GetComponentRotation();
		const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 6.f);  // 6.f // 10.f
		TurnInPlace_Yaw = TargetRotation.Yaw - NewRotation.Yaw;

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

void AControllableCharacter::UpdateCameraBoomOffset(float DeltaTime, bool bReset)
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

void AControllableCharacter::UpdateCameraBoomLength(float DeltaTime, bool bReset)
{
	const float SpeedFraction = GetVelocity().Size() / GetCharacterMovement()->MaxWalkSpeed;
	const float NewTargetArmLength = BaseCameraBoomLength + (SpeedFraction * CameraBoomLengthAdditiveFactor);
	
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, NewTargetArmLength, DeltaTime, CameraSpeed / 2);
}

void AControllableCharacter::UpdateCameraBoomRotation(float DeltaTime, bool bReset)
{
	FHitResult OutHit;
	UKismetSystemLibrary::SphereTraceSingle(this, CameraBoomTopPoint->GetComponentLocation(), CameraBoomTopPoint->GetComponentLocation() +
		(CameraBoomTopPoint->GetForwardVector() * 2000.f), 5.f/*TraceRadius*/, ETraceTypeQuery::TraceTypeQuery1, false, {}, EDrawDebugTrace::None,
		OutHit, false);

	if (OutHit.bBlockingHit && OutHit.GetActor() != this) {
		float RotateDirection = 0.f;
		{
			FVector Start = CameraBoomTopPoint->GetComponentLocation();
			FVector Forward = CameraBoomTopPoint->GetForwardVector();
			FVector LeftDirection = Forward.RotateAngleAxis(-30.f, FVector::UpVector);
			FVector RightDirection = Forward.RotateAngleAxis(30.f, FVector::UpVector);

			FHitResult LeftHit, RightHit;
			TArray<AActor*> ActorsToIgnore = { this };

			// Left trace
			UKismetSystemLibrary::LineTraceSingle(
				this,
				Start,
				Start + LeftDirection * 2000.f,
				ETraceTypeQuery::TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				LeftHit,
				true
			);

			// Right trace
			UKismetSystemLibrary::LineTraceSingle(
				this,
				Start,
				Start + RightDirection * 2000.f,
				ETraceTypeQuery::TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				RightHit,
				true
			);
			float LeftDist = LeftHit.bBlockingHit ? LeftHit.Distance : 2000.f;
			float RightDist = RightHit.bBlockingHit ? RightHit.Distance : 2000.f;

			if (LeftDist > RightDist + 50.f) // Slight bias
			{
				RotateDirection = -1.f; // Rotate left
			}
			else if (RightDist > LeftDist + 50.f) {
				RotateDirection = 1.f; // Rotate right
			}
		}

		FRotator NewCameraBoomRotation = CameraBoom->GetRelativeRotation();
		NewCameraBoomRotation.Yaw = FMath::FInterpTo(NewCameraBoomRotation.Yaw, NewCameraBoomRotation.Yaw - (20 * RotateDirection)/*CameraBoomRotation_Factor*/, DeltaTime, CameraSpeed / 2);
		CameraBoom->SetRelativeRotation(NewCameraBoomRotation);
		CameraBoom_Secondary->SetRelativeRotation(NewCameraBoomRotation);
	}
	else if (GetVelocity().Size() > 15.f/*CameraBoomBaseRotation_VelocityThreshold*/) {
		FRotator NewCameraBoomRotation = CameraBoom->GetRelativeRotation();
		NewCameraBoomRotation.Yaw = FMath::FInterpTo(NewCameraBoomRotation.Yaw, BaseCameraBoomRotationYaw, DeltaTime, CameraSpeed);
		CameraBoom->SetRelativeRotation(NewCameraBoomRotation);
		CameraBoom_Secondary->SetRelativeRotation(NewCameraBoomRotation);
	}
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
	if (PawnActionState >= EPawnActionState::EPAS_SelectingItem) return;
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

void AControllableCharacter::GamepadCursorMove(const FInputActionValue& Value)
{
	if (!PlayerController) return;

	float X, Y;
	PlayerController->GetMousePosition(X, Y);
	int32 ViewportX, ViewportY;
	PlayerController->GetViewportSize(ViewportX, ViewportY);
	const FVector2d InputValue = Value.Get<FVector2d>();
	float NewX = FMath::Clamp(X + (InputValue.X * GamepadCursorMoveSpeed), 0.f, (float)ViewportX);
	float NewY = FMath::Clamp(Y + (InputValue.Y * GamepadCursorMoveSpeed), 0.f, (float)ViewportY);
	PlayerController->SetMouseLocation(NewX, NewY);
}

void AControllableCharacter::ToggleFlashlight()
{
	if (FlashlightComponent) FlashlightComponent->SetVisibility(!FlashlightComponent->IsVisible());
}

void AControllableCharacter::ToggleMovementType()
{
	switch (MovementType)
	{
	case EMovementType::EMT_NONE:
		break;
	case EMovementType::EMT_WALK:
		MovementType = EMovementType::EMT_JOG;
		break;
	case EMovementType::EMT_JOG:
		MovementType = EMovementType::EMT_WALK;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = MovementType == EMovementType::EMT_WALK ?
		GetCharacterMovement()->MaxWalkSpeedCrouched : GetCharacterMovement()->MaxCustomMovementSpeed;
}

void AControllableCharacter::ResetMovementSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MovementType == EMovementType::EMT_WALK ?
		GetCharacterMovement()->MaxWalkSpeedCrouched : GetCharacterMovement()->MaxCustomMovementSpeed;
}

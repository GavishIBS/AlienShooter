// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Characters/BaseCharacter.h"
#include "OptionTypes.h"
#include "ControllableCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpotLightComponent;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AControllableCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AControllableCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	// Controller
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Controller)
	TObjectPtr<APlayerController> PlayerController;

	void CheckCursorLocation();

	/* Character rotation begin */
	void UpdateCharacterRotationToCursor();
	/* Character rotation end */

	/* Camera boom begin */
	void UpdateCameraBoomOffset(float DeltaTime, bool bReset = false);
	void UpdateCameraBoomLength(float DeltaTime, bool bReset = false);
	void UpdateCameraBoomRotation(float DeltaTime, bool bReset = false);

	inline static float BaseCameraBoomLength = 1000.f;
	inline static float BaseCameraBoomRotationYaw = -40.f;
	
	UPROPERTY(EditAnywhere, Category=Properties)
	float CameraBoomLengthAdditiveFactor = 75.f;

	FCameraOptions CameraSettings{true, true};
	/* Camera boom end */

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpotLightComponent> FlashlightComponent;

protected:
	/* Input callbacks begin */
	void Move(const FInputActionValue& Value);
	void GamepadCursorMove(const FInputActionValue& Value);
	void ToggleFlashlight();
	void ToggleMovementType();
	/* Input callbacks end */

	void ResetMovementSpeed();
	
	/* Input actions begin */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> GamepadCursorMoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> ToggleFlashlightAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> ToggleMovementTypeAction;
	/* Input end */

	// Properties
	UPROPERTY(EditAnywhere, Category = "Properties")
	float RestrictedRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float TurnInPlace_Yaw = 0.f;

	FVector CursorTraceHitLocation = FVector();
	bool CursorOverlappingHittable = false;

	/* Camera movements begin */
	FVector2D CursorPosition_LastFrame = FVector2D();
	FVector CurrentCameraBoomOffset = FVector();
	FVector CameraBoomOffset_LastFrame = FVector();

	//UPROPERTY(EditAnywhere, Category = "Camera Movements")
	//int MaxCameraOffsetX = 850; // 1000   // Max offset camera can have in relative X direction
	
	//UPROPERTY(EditAnywhere, Category = "Camera Movements")
	//int MaxCameraOffsetY = 750; // 900    // Max offset camera can have in relative Y direction
	
	UPROPERTY(EditAnywhere, Category = "Camera Movements")
	float CameraOffsetFactor = 300.f;

	UPROPERTY(EditAnywhere, Category = "Camera Movements")
	int CameraResponse_InnerRadius = 420;   // Radius in which camera doesn't respond to cursor position, unless it have a offset in opposite direction
	
	UPROPERTY(EditAnywhere, Category = "Camera Movements")
	float CameraResponse_RadiusLease = 20.f;

	UPROPERTY(EditAnywhere, Category = "Camera Movements")
	float CameraSpeed = 3.f;
	/* Camera movements end */

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (ClampMin = "15"))
	float GamepadCursorMoveSpeed = 35.f;

public:
	FORCEINLINE FVector GetCursorTraceHitLocation() const {return CursorTraceHitLocation;}
	FORCEINLINE bool IsCursorOverlappingHittable() const { return CursorOverlappingHittable; }
	FORCEINLINE APlayerController* GetPlayerController() const { return PlayerController; }

	FORCEINLINE float GetTurnInPlace_Yaw() const { return TurnInPlace_Yaw; }
};

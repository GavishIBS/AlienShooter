// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Characters/BaseCharacter.h"
#include "ControllableCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

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

	/* Character rotation begin */
	void UpdateCharacterRotationToCursor();
	/* Character rotation end */
	
protected:
	/* Input callbacks begin */
	void Move(const FInputActionValue& Value);
	/* Input callbacks end */
	
	/* Input actions begin */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> MoveAction;
	/* Input end */

	FVector CursorTraceHitLocation = FVector();

public:
	FORCEINLINE FVector GetCursorTraceHitLocation() const {return CursorTraceHitLocation;}
	FORCEINLINE APlayerController* GetPlayerController() const { return PlayerController; }
};

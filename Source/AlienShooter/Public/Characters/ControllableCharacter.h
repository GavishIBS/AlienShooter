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

protected:
	virtual void BeginPlay() override;

	// Controller
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Controller)
	TObjectPtr<APlayerController> PlayerController;

	/* Character rotation begin */
	void UpdateCharacterRotationToCursor() const;
	/* Character rotation end */
	
protected:
	/* Input callbacks begin */
	void Move(const FInputActionValue& Value);
	void LeftMouseButtonPressed();
	/* Input callbacks end */
	
	/* Input actions begin */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> LeftMouseButtonAction;
	/* Input end */

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

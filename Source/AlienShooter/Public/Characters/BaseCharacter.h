// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "../GameCharacter.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAttributeComponent;
class UInventoryComponent;
class URadioPhoneComponent;

UCLASS()
class ALIENSHOOTER_API ABaseCharacter : public AGameCharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void DoCameraShake(const FVector IntensityFactor, const float ShakeTime, const bool bShakeInDir, const FVector Direction);

	void Rebirth(const FTransform& RebirthTransform);

protected:
	virtual void BeginPlay() override;
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass) override;

	/* Camera begin */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<UCameraComponent> ViewCamera;
	/* Camera end */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URadioPhoneComponent> RadioPhoneComponent;

	// Properites
	EPawnActionState PawnActionState = EPawnActionState::EPAS_Idle;

private:
	FVector CameraOriginalLocation_Rel;
	FVector CameraTargetLocation_Rel = CameraOriginalLocation_Rel;
	FTimerHandle CameraShakeTimerHandle;
	FTimerHandle CameraShakeStopTimerHandle;

	FVector CameraShakeIntensity = {1, 1, 1};
	bool bShakeInDirection = false;
	FVector ShakeDirection = FVector(0, 0, 0);

	void GenerateRandomCameraLocation();
	void StopCameraShake();

public:





	FORCEINLINE UAttributeComponent* GetAttributeComponent() const { return AttributeComponent; }
};

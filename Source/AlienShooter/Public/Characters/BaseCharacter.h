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
class UObjectiveComponent;

UENUM(BlueprintType)
enum class EReactionType : uint8
{
	None, Combat, Hurt, Combat_Sarcasm
};

USTRUCT()
struct FReactionDialogues
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Data")
	EReactionType Reaction = EReactionType::None;
	
	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<TObjectPtr<USoundBase>> DialogueAudios;
	
	UPROPERTY(EditAnywhere, Category = "Data", meta=(Min="0", Max="100"))
	int32 TriggerChancePercent = 20;
};

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
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType = nullptr) override;

	/* Camera begin */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<USpringArmComponent> CameraBoom_Secondary;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<USceneComponent> CameraBoomTopPoint;

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

protected:
	void TriggerSelfDialogue(const EReactionType InReactionType);
	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<FReactionDialogues> ReactionDialoguesData;

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


protected:
	UPROPERTY(BlueprintReadOnly)
	FString PlayerName = "TestPlayerName";
public:
	FORCEINLINE FString GetPlayerName() const { return PlayerName; }
	FORCEINLINE void SetPlayerName(const FString& InPlayerName) { PlayerName = InPlayerName; }
};

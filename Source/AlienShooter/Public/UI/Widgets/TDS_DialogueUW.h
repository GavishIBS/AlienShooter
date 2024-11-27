// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_DialogueUW.generated.h"

class UDialogueComponent;
class USoundWave;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueCompleted);  // Refers to completion of dialogue printing
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextButtonDown);  // Refers next button pressed event

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_DialogueUW : public UTDSUserWidget
{
	GENERATED_BODY()
	
public:
	friend UDialogueComponent;
	UTDS_DialogueUW() = default;
	UTDS_DialogueUW(const FObjectInitializer& ObjectInitializer);

	virtual void InitComponent() override;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueCompleted OnDialogueCompleted;
	UPROPERTY(BlueprintAssignable)
	FOnNextButtonDown OnNextButtonDown;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* NextButton;

	UFUNCTION()
	virtual void OnNextButtonClicked();

protected:
	APawn* DialogueInitiator = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FString TextOnScreen = FString();

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FString TargetText = FString();

	USoundWave* AudioWave = nullptr;

private:
	FTimerHandle PrintingTimerHandle;

private:
	void StartWritingDialogue();

	FORCEINLINE void IncrementCharacter();

	FORCEINLINE void SetTargetText(const FString& InText) { TargetText = InText; };
	FORCEINLINE void SetDialogueAudio(USoundWave* InSoundWave) { AudioWave = InSoundWave; }
	
public:
	FORCEINLINE void SetTargetTextAndDialogueAudio(const FString& InText, USoundWave* InSoundWave) {
		SetTargetText(InText);
		SetDialogueAudio(InSoundWave);
		StartWritingDialogue();
	}

	FORCEINLINE void SetDialogueInitiator(APawn* InPawn) { DialogueInitiator = InPawn; }
};

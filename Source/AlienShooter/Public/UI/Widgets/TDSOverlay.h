// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDSOverlay.generated.h"

class UTDSWeaponBoxList;
class AShooterCharacter;
class UTDSWeaponInHandDataBox;
class UCanvasPanel;
class UProgressBar;
class UBorder;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDSOverlay : public UTDSUserWidget
{
	GENERATED_BODY()

public:
	UTDSOverlay() = default;
	UTDSOverlay(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void InitComponents();
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTDSWeaponInHandDataBox> WeaponInHandDataBox;

	//UPROPERTY(meta=(BindWidget))
	//TObjectPtr<UTDSWeaponBoxList> WeaponBoxList;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* EnvText_Border;

protected:
	UFUNCTION()
	void UpdateHealthBar(float Health, float MaxHealth);

protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	float Environment_Subtitles_Text_ClearDelay = 2.f;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FText EnvironmentDialogue_Subtitles_Text = FText();

private:
	FTimerHandle EnvironmentDialogueText_Subtitles_TimerHandle;
	void OnEnvironmentDialogueCompleted();

	FTimerHandle TypeEnvironmentDialogue_Subtitles_Text_TimerHandle;
	FText Target_Subtitles_Text = FText();
	void IncrementCharacter_To_Subtitles_Text();

public:
	void SetEnvironmentDialogue_Subtitles_Text(const FText InText, const float& DialogueAudioDuration);
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_DialogueLogSlot_UserWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_DialogueLogSlot_UserWidget : public UTDSUserWidget
{
	GENERATED_BODY()
	
public:
	UTDS_DialogueLogSlot_UserWidget() = default;
	UTDS_DialogueLogSlot_UserWidget(const FObjectInitializer& ObjectInitializer);

	void SetDialogueLog_Values(const FName& InSpeakerName, const FString& InDialogueLogText);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void BindCallbacks();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SpeakerName_TextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Dialogue_TextBlock;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FName DialogueSpeakerName = FName("SpeakerName");
	
	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FString DialogueLogText = FString("sample dialogue line");
};

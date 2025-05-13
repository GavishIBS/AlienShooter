// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "Dialogue_Types.h"
#include "TDS_DialogueLogsUserWidget.generated.h"

class UTDS_DialogueLogSlot_UserWidget;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_DialogueLogsUserWidget : public UTDSUserWidget
{
	GENERATED_BODY()

public:
	UTDS_DialogueLogsUserWidget() = default;
	UTDS_DialogueLogsUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void BindCallbacks();

	UPROPERTY(BlueprintReadOnly) // DEBUG // TEMP
	TArray<FDialogueLog> DialogueLogs;

	UFUNCTION(BlueprintImplementableEvent)
	void InitComponentWithText();    // DEBUG // TEMP

	UFUNCTION(BlueprintImplementableEvent)
	void InitComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<UTDS_DialogueLogSlot_UserWidget> DialogueLogSlot_UW_Class = nullptr;
	
public:
	FORCEINLINE void SetDialogueLogs(const TArray<FDialogueLog>& InDialogueLogs) { DialogueLogs = InDialogueLogs; InitComponentWithText(); if (DialogueLogSlot_UW_Class) { InitComponent(); } }
};

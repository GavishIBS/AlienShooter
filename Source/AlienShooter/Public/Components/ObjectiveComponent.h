// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectiveTypes.h"
#include "Dialogue_Types.h"
#include "ObjectiveComponent.generated.h"

class UTDS_DialogueLogsUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UObjectiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//friend class AShooterCharacter;
	UObjectiveComponent();
	
	// Radio Phone Dialogue Logs BEGIN
	void RemoveDialogueLogsMenu_UW();
	void SetDialogueLogs_Wrapper();
	void AddRadioPhoneDialogueLog(const FDialogueLog&& NewLog);
	// Radio Phone Dialogue Logs END
	
	// Objectives BEGIN
	UFUNCTION(BlueprintCallable)  // TODO: TEMP
	bool AddObjective(const FRemindableObjective& Objective);
	bool CompleteObjective(const FObjective& Objective);
	std::tuple<bool, FReminderInfo> TriggerReminder();
	// Objectives END

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FObjective> DisplayObjectives;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FReminderInfo> RemindObjectives;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FDialogueLog> RadioPhoneDialogueLogs;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<UTDS_DialogueLogsUserWidget> DialogueLogMenu_UWClass = nullptr;

	UTDS_DialogueLogsUserWidget* DialogueLogsMenu_UW_Ref = nullptr;

public:
	FORCEINLINE TSubclassOf<UTDS_DialogueLogsUserWidget> GetDialogueLogMenu_UWClass() const { return DialogueLogMenu_UWClass; }
	
	FORCEINLINE bool Is_DialogueLogs_UW_Valid() const { return (bool)DialogueLogsMenu_UW_Ref; }
	
	FORCEINLINE void SetDialogueLogsMenu_UW_Ref(UTDS_DialogueLogsUserWidget* In_UW) { DialogueLogsMenu_UW_Ref = In_UW; }
};

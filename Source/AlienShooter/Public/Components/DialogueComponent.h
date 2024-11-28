// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dialogue_Types.h"
#include "DialogueComponent.generated.h"

class UTDSUserWidget;
class UTDS_DialogueUW;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConversationEnd);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();

	void StartConversation(UTDS_DialogueUW* InDialogueScreenUW);

	UPROPERTY(BlueprintAssignable)
	FOnConversationEnd OnConversationEnd;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartNextDialogue();

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<FDialogueData> ConversationData;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<UTDSUserWidget> DialogueScreenUW_Class;

	UTDS_DialogueUW* DialogueScreenUW;

	int CurrentDialogueIndex = -1;

public:
	FORCEINLINE bool IsConvoEnd(const int InIndex);
	FORCEINLINE FDialogueData GetNextDialogueData(const int InIndex);
	FORCEINLINE TSubclassOf<UTDSUserWidget> GetDialogueScreenUWClass() const { return DialogueScreenUW_Class; }
};

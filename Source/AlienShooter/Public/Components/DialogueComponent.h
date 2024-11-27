// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dialogue_Types.h"
#include "DialogueComponent.generated.h"

class UTDSUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<FDialogueData> ConversationData;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<UTDSUserWidget> DialogueScreenUW_Class;

public:
	FORCEINLINE bool IsConvoEnd(const int InIndex);
	FORCEINLINE FDialogueData GetNextDialogueData(const int InIndex);
	FORCEINLINE TSubclassOf<UTDSUserWidget> GetDialogueScreenUWClass() const { return DialogueScreenUW_Class; }
};

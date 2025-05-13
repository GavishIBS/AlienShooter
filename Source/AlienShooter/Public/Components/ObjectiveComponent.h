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
	friend class ABaseCharacter;
	friend class AShooterCharacter;
	friend class AShooterCharacter_MeleeAttacker;
	UObjectiveComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FObjective> DisplayObjectives;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FDialogueLog> RadioPhoneDialogueLogs;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<UTDS_DialogueLogsUserWidget> DialogueLogMenu_UWClass = nullptr;

	UTDS_DialogueLogsUserWidget* DialogueLogsMenu_UW_Ref = nullptr;
};

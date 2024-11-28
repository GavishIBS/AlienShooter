// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DialoguePawn.generated.h"

class UBoxComponent;
class UDialogueComponent;
class UTDS_DialogueUW;
class AControllableCharacter;

UCLASS()
class ALIENSHOOTER_API ADialoguePawn : public APawn
{
	GENERATED_BODY()

public:
	ADialoguePawn();

protected:
	virtual void BeginPlay() override;
	virtual void InitCallbacks();

	UFUNCTION()
	void StartConversation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnConvoEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> PawnMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> ConversationArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UDialogueComponent> DialogueComponent;

	UTDS_DialogueUW* DialogueScreenUW;

	AControllableCharacter* SecondPerson;
};

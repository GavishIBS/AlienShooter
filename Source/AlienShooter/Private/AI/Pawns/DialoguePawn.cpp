// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/Pawns/DialoguePawn.h"
#include "Components/BoxComponent.h"
#include "Components/DialogueComponent.h"
#include "Characters/ShooterCharacter.h"
#include "HUD/TDSHUD.h"
#include "UI/Widgets/TDS_DialogueUW.h"

ADialoguePawn::ADialoguePawn()
{
	PrimaryActorTick.bCanEverTick = false;

	PawnMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PawnMesh"));
	PawnMesh->SetupAttachment(GetRootComponent());

	ConversationArea = CreateDefaultSubobject<UBoxComponent>(TEXT("ConversationArea"));
	ConversationArea->SetupAttachment(PawnMesh);

	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
}

void ADialoguePawn::BeginPlay()
{
	Super::BeginPlay();
	InitCallbacks();
}

void ADialoguePawn::InitCallbacks()
{
	if (ConversationArea) {
		ConversationArea->OnComponentBeginOverlap.AddDynamic(this, &ADialoguePawn::StartConversation);
	}
}

void ADialoguePawn::StartConversation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Optimization -> Use a better approach to trigger conversation and create widget in viewport

	if (OtherActor->ActorHasTag("Player")) {
		if (AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor)) {
			ATDSHUD* TDSHUD = Cast<ATDSHUD>(Character->GetPlayerController()->GetHUD());
			if (TDSHUD) {
				DialogueScreenUW = Cast<UTDS_DialogueUW>(TDSHUD->MakeWidgetInViewport(DialogueComponent->GetDialogueScreenUWClass()));
				DialogueScreenUW->SetDialogueInitiator(this);
				DialogueComponent->StartConversation(DialogueScreenUW);
			}
		}
	}
	else return;
}

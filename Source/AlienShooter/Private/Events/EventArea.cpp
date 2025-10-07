// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Events/EventArea.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"

AEventArea::AEventArea()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapArea = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapArea"));
	//OverlapArea->SetupAttachment(GetRootComponent());
	SetRootComponent(OverlapArea);

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(OverlapArea);
}

void AEventArea::BeginPlay()
{
	Super::BeginPlay();
	BindCallbacks();
}

void AEventArea::BindCallbacks()
{
	if (OverlapArea) {
		OverlapArea->OnComponentBeginOverlap.AddDynamic(this, &AEventArea::OnOverlap);
		OverlapArea->OnComponentEndOverlap.AddDynamic(this, &AEventArea::OnEndOverlap);
	}
}

void AEventArea::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bListenOverlaps == false) return;

	if (OtherActor->ActorHasTag(TagToLook)) {
		OnEnterArea.Broadcast(OtherActor);
		if (ChainedEvent) {
			FTimerHandle ChainedEvent_TimerHandle;
			GetWorldTimerManager().SetTimer(ChainedEvent_TimerHandle,
				FTimerDelegate::CreateLambda([this, OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult]() {
					ChainedEvent->OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
				}),
				ChainedEvent_Delay, false, ChainedEvent_Delay);
		}
	}
}

void AEventArea::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bListenOverlaps == false) return;

	if (OtherActor->ActorHasTag(TagToLook)) {
		OnExitArea.Broadcast(OtherActor);
		if (ChainedEvent) {
			FTimerHandle ChainedEvent_TimerHandle;
			GetWorldTimerManager().SetTimer(ChainedEvent_TimerHandle,
				FTimerDelegate::CreateLambda([this, OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex]() {
					ChainedEvent->OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
					}),
				ChainedEvent_Delay, false, ChainedEvent_Delay);
		}
	}
}

bool AEventArea::SetListenOverlapWithDelay(const bool InValue, const float InDelay)
{
	FTimerHandle UpdateListenOverlapTimerHandle;
	GetWorldTimerManager().SetTimer(UpdateListenOverlapTimerHandle, FTimerDelegate::CreateLambda([this, InValue] {
		bListenOverlaps = InValue;
	}), InDelay, false);
	return true;
}

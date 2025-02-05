// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Events/EventArea.h"
#include "Components/BoxComponent.h"

AEventArea::AEventArea()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapArea = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapArea"));
	OverlapArea->SetupAttachment(GetRootComponent());
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
	if (OtherActor->ActorHasTag(TagToLook)) {
		OnEnterArea.Broadcast(OtherActor);
	}
}

void AEventArea::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TagToLook)) {
		OnExitArea.Broadcast(OtherActor);
	}
}

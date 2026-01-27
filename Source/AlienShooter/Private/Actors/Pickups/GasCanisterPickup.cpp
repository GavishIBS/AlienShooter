// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/GasCanisterPickup.h"
#include "Game/MainGameInstance.h"

AGasCanisterPickup::AGasCanisterPickup()
{
}

void AGasCanisterPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AGasCanisterPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;

	UMainGameInstance* MainGI = Cast<UMainGameInstance>(GetGameInstance());
	if (!IsValid(MainGI)) return;

	MainGI->AddGasCanister(1);

	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

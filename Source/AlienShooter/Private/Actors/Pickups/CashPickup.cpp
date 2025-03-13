// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/CashPickup.h"
#include "Kismet/GameplayStatics.h"
#include "Game/MainGameInstance.h"

ACashPickup::ACashPickup()
{
}

void ACashPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ACashPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;
	UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (MainGameInstance) {
		MainGameInstance->AddCash(Amount);
	}
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

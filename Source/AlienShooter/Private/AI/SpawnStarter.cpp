// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/SpawnStarter.h"
#include "Components/BoxComponent.h"
#include "AI/AISpawnerBase.h"

ASpawnStarter::ASpawnStarter()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	OverlapBox->SetupAttachment(GetRootComponent());
}

void ASpawnStarter::BeginPlay()
{
	Super::BeginPlay();
	if (!Spawner) Destroy();
	if (IsValid(this) && OverlapBox)
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawnStarter::OnBoxOverlap);
}

void ASpawnStarter::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Spawner) Destroy();
	if (OtherActor->ActorHasTag("Player"))
	{
		bool bSpawned = Spawner->StartSpawning();
		if (bSpawned) Destroy();
	}
	
}

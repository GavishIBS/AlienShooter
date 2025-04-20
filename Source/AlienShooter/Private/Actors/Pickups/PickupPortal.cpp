// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/PickupPortal.h"
#include "Components/StaticMeshComponent.h"
#include "Actors/Pickups/Pickup.h"

APickupPortal::APickupPortal()
{
	PrimaryActorTick.bCanEverTick = false;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(GetRootComponent());

	ItemSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSpawnPoint"));
	ItemSpawnPoint->SetupAttachment(PortalMesh);
}

void APickupPortal::BeginPlay()
{
	Super::BeginPlay();
	SpawnItem();
}

void APickupPortal::SpawnItem()
{
	if (!PickupItemClass) return;

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters ActorSpawnParams;
		APickup* SpawnedPickup = World->SpawnActor<APickup>(PickupItemClass, ItemSpawnPoint->GetComponentLocation(), ItemSpawnPoint->GetComponentRotation(), ActorSpawnParams);
		if (SpawnedPickup) {
			SpawnedPickup->OnDestroyed.AddDynamic(this, &APickupPortal::SpawnItemWithDelay);
		}
	}
}

void APickupPortal::SpawnItemWithDelay(AActor* LastDestroyedPickup)
{
	FTimerHandle SpawnTimerHandle;
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &APickupPortal::SpawnItem, SpawnDelay, false, SpawnDelay);
}

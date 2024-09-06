// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/InventoryPickup.h"
#include "Components/InventoryComponent.h"
#include "Components/SphereComponent.h"

AInventoryPickup::AInventoryPickup()
{
	OverlapSphere->SetGenerateOverlapEvents(false);
}

void AInventoryPickup::BeginPlay()
{
	Super::BeginPlay();
	OverlapSphere->SetGenerateOverlapEvents(bGenerateOverlapEvents);
}

bool AInventoryPickup::PostPickup(AActor* Caller)
{
	if (!ItemData.ItemClass) return true;
	if (UInventoryComponent* InventoryComponent = Caller->FindComponentByClass<UInventoryComponent>())
	{
		if (InventoryComponent->AddItemToInventory(ItemData))
			return true;
	}
	return false;
}

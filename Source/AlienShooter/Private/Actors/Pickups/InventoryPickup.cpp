// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/InventoryPickup.h"
#include "Components/InventoryComponent.h"

AInventoryPickup::AInventoryPickup()
{
}

void AInventoryPickup::BeginPlay()
{
	Super::BeginPlay();
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

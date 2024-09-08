// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/LoakableElectronicDoor.h"
#include "Components/InventoryComponent.h"
#include "Actors/Pickups/InventoryPickup.h"

ALoakableElectronicDoor::ALoakableElectronicDoor()
{
}

void ALoakableElectronicDoor::BeginPlay()
{
	Super::BeginPlay();
}

void ALoakableElectronicDoor::Tick(float DeltaTime)
{
	if (bLocked) return;
	Super::Tick(DeltaTime);
}

void ALoakableElectronicDoor::OnLockOpened()
{
}

bool ALoakableElectronicDoor::TryUnlockDoor(AActor* Caller)
{
	if (!DoorKey_Class) return false;

	if (UInventoryComponent* InventoryComponent = Caller->FindComponentByClass<UInventoryComponent>())
	{
		if (InventoryComponent->HaveItem(DoorKey_Class) > -1)
		{
			if (InventoryComponent->RemoveItemFromInventory(DoorKey_Class, true))
			{
				OpenLock();
				return true;
			}
		}
	}
	return false;
}

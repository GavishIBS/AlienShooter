// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/InventoryComponent.h"
#include "Actors/Pickups/InventoryPickup.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::InitInventory()
{
	Content.SetNum(InventorySize);
}

bool UInventoryComponent::AddItemToInventory(const FItemData ItemData)
{
	int32 Index = HaveEmpltySlot();
	if (Index > -1)
	{
		if (Content.Num() <= Index) return false;
		Content[Index].ItemData = ItemData;
		Content[Index].bEmpty = false;
		return true;
	}
	return false;
}

bool UInventoryComponent::RemoveItemFromInventory(const TSubclassOf<AInventoryPickup> ItemClass)
{
	int Index = HaveItem(ItemClass);
	if (Index > -1)
	{
		if (Content.Num() <= Index) return false;
		Content[Index].bEmpty = true;
		Content[Index].ItemData = FItemData();
		return true;

	}
	return false;
}

int32 UInventoryComponent::HaveEmpltySlot()
{
	int Counter = 0;
	for (auto Slot : Content)
	{
		if (Slot.bEmpty)
			return Counter;
		Counter++;
	}
	return -1;
}

int UInventoryComponent::HaveItem(const TSubclassOf<AInventoryPickup> ItemClass)
{
	int Counter = 0;
	for (auto Slot : Content)
	{
		if (Slot.bEmpty == false && Slot.ItemData.ItemClass == ItemClass)
			return Counter;
		Counter++;
	}
	return -1;
}

void UInventoryComponent::ClearSlot(const int32 Index)
{
	if (Content.Num() <= Index) return;
	Content[Index].bEmpty = true;
	Content[Index].ItemData = FItemData();
}

void UInventoryComponent::PrintContent()
{
	int Counter = 0;
	for (auto Slot : Content)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Item number: %d -> %s"),
				Counter, *Slot.ItemData.ItemName.ToString()));
		}
		Counter++;
	}
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/InventoryComponent.h"
#include "Actors/Pickups/InventoryPickup.h"
#include "GameFramework/Character.h"
#include "Actors/UsableActor.h"
#include "UI/Widgets/TDS_InventoryMenu.h"

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
	OwnerAsCharacter = Cast<ACharacter>(GetOwner());
}

bool UInventoryComponent::AddItemToInventory(const FItemData ItemData)
{
	int32 Index = HaveEmpltySlot();
	if (Index > -1)
	{
		if (Content.Num() <= Index) return false;
		Content[Index].ItemData = ItemData;
		Content[Index].bEmpty = false;
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventoryComponent::RemoveItemFromInventory(const TSubclassOf<AInventoryPickup> ItemClass, bool bShouldDestroy)
{
	int Index = HaveItem(ItemClass);
	if (Index > -1)
	{
		if (Content.Num() <= Index) return false;
		Content[Index].bEmpty = true;
		Content[Index].ItemData = FItemData();
		OnInventoryUpdated.Broadcast();
		if (bShouldDestroy && Index == SelectedItemIndex)
		{
			DestroyCurrentItem();
		}
		return true;

	}
	return false;
}

bool UInventoryComponent::RemoveItemFromInventory(const int32 Index, bool bShouldDestroy)
{
	if (Index >= Content.Num()) return false;
	Content[Index].bEmpty = true;
	Content[Index].ItemData = FItemData();
	OnInventoryUpdated.Broadcast();
	if (bShouldDestroy && Index == SelectedItemIndex)
	{
		DestroyCurrentItem();
	}
	return true;
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
	OnInventoryUpdated.Broadcast();
}

bool UInventoryComponent::SelectItem(const int Index)
{
	if (Index == SelectedItemIndex) return false;
	if (!OwnerAsCharacter || !GetWorld()) return false;
	if (Index < 0 || InventorySize - 1 < Index) return false;

	// Destroy current item
	DestroyCurrentItem();

	const TSubclassOf<AActor> ItemClassSelection = Content[Index].ItemData.ItemClass;
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = (APawn*)OwnerAsCharacter;
	ActorSpawnParameters.Owner = (AActor*)OwnerAsCharacter;
	AInventoryPickup* ItemSelection = GetWorld()->SpawnActor<AInventoryPickup>(ItemClassSelection, ActorSpawnParameters);

	if (!ItemSelection)
	{
		SelectedItemIndex = Index;
		return false;
	}

	// Update ownership and broadcast OnItemSelected
	ItemSelection->SetOwner((AActor*)OwnerAsCharacter);
	OnItemSelected.Broadcast(ItemSelection);

	// Item attachment
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	ItemSelection->AttachToComponent(OwnerAsCharacter->GetMesh(), AttachmentTransformRules,
		ItemSelection->GetAttachmentSocketName());
	ItemInHand = ItemSelection;
	SelectedItemIndex = Index;

	return true;
}

bool UInventoryComponent::UnselectItem()
{
	// Destroy current item
	DestroyCurrentItem();
	return true;
}

void UInventoryComponent::UseButtonPressed()
{
	if (!ItemInHand) return;

	if (AUsableActor* UsableActor = Cast<AUsableActor>(ItemInHand))
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (UsableActor->UseItem(Character))
		{
			RemoveItemFromInventory(SelectedItemIndex);
			ItemInHand = nullptr;
		}
	}
}

void UInventoryComponent::DestroyCurrentItem()
{
	if (ItemInHand)
	{
		ItemInHand->Destroy();
		ItemInHand = nullptr;
	}
}

void UInventoryComponent::SetInventory_UW(UTDS_InventoryMenu* UW)
{
	InventoryMenu_UW = UW;
	if (InventoryMenu_UW)
	{
		InventoryMenu_UW->CreateSlots(Content);
	}
}

void UInventoryComponent::CloseInventory_UI()
{
	if (InventoryMenu_UW)
		InventoryMenu_UW->RemoveFromParent();
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

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

bool UInventoryComponent::AddItemToInventoryFromClass(TSubclassOf<AActor> InItemClass)
{
	if (InItemClass == nullptr) return false;

	FActorSpawnParameters ActorSpawnParameters;
	//ActorSpawnParameters.Instigator = OwnerAsShooterCharacter;
	ActorSpawnParameters.Owner = GetOwner();
	AInventoryPickup* Item = GetWorld()->SpawnActor<AInventoryPickup>(InItemClass, ActorSpawnParameters);
	return AddItemToInventory(Item->GetItemData());
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString("Try select item"));
	if (Index == SelectedItemIndex) return false;
	if (!OwnerAsCharacter || !GetWorld()) return false;
	if (Index < 0 || InventorySize - 1 < Index) return false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString("Selecting item"));

	// Destroy current item
	//DestroyCurrentItem();

	if (Content[Index].bEmpty)
	{
		if (Index > SelectedItemIndex)
		{
			if (SelectItem(Index+1))
				return true;
			else return false;
		}
		else if (Index < SelectedItemIndex)
		{
			if (SelectItem(Index - 1))
				return true;
			else return false;
		}
	}

	const TSubclassOf<AActor> ItemClassSelection = Content[Index].ItemData.ItemClass;
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = (APawn*)OwnerAsCharacter;
	ActorSpawnParameters.Owner = (AActor*)OwnerAsCharacter;
	AInventoryPickup* ItemSelection = GetWorld()->SpawnActor<AInventoryPickup>(ItemClassSelection, ActorSpawnParameters);

	if (!ItemSelection)
	{
		//SelectedItemIndex = Index;
		if (InventoryMenu_UW)
			InventoryMenu_UW->SetSelectedIndex(SelectedItemIndex);
		return false;
	}

	// Update ownership and broadcast OnItemSelected
	ItemSelection->SetOwner((AActor*)OwnerAsCharacter);
	OnItemSelected.Broadcast(ItemSelection);

	// Destroy current item
	DestroyCurrentItem();

	// Item attachment
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	ItemSelection->AttachToComponent(OwnerAsCharacter->GetMesh(), AttachmentTransformRules,
		ItemSelection->GetAttachmentSocketName());
	ItemInHand = ItemSelection;
	SelectedItemIndex = Index;
	if (InventoryMenu_UW)
		InventoryMenu_UW->SetSelectedIndex(SelectedItemIndex);

	return true;
}

bool UInventoryComponent::UnselectItem()
{
	// Destroy current item
	DestroyCurrentItem();

	ItemInHand = nullptr;
	SelectedItemIndex = -1;
	
	return true;
}

void UInventoryComponent::ClearItemBeingUsed()
{
	ItemBeingUsed = nullptr;
}

bool UInventoryComponent::IsItemUsableOnIndex(const int Index)
{
	if (Index >= Content.Num()) return false;

	if (Content[Index].bEmpty == false &&
		Content[Index].ItemData.bUsable == true) return true;
	else return false;
}

AUsableActor* UInventoryComponent::UseButtonPressed()
{
	if (!ItemInHand) return 0;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString("ITEM_HAND is valid!"));

	if (AUsableActor* UsableActor = Cast<AUsableActor>(ItemInHand))
	{
		//ACharacter* Character = Cast<ACharacter>(GetOwner());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString("Trying to use item now"));
		if (AUsableActor* Usable = UsableActor->UseItem(OwnerAsCharacter))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString("AUsableActor::UseItem is called with return = true"));
			RemoveItemFromInventory(SelectedItemIndex);
			ItemInHand = nullptr;
			ItemBeingUsed = Usable;
			return Usable;
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString("AUsableActor::UseItem is called with return = false"));
		}
	}

	return 0;
}

void UInventoryComponent::DestroyCurrentItem()
{
	if (ItemInHand)
	{
		ItemInHand->Destroy();
		ItemInHand = nullptr;
	}
}

void UInventoryComponent::OnItemUsed()
{
	if (ItemBeingUsed) {
		ItemBeingUsed->OnItemUsed();
		ItemBeingUsed = nullptr;
	}
}

void UInventoryComponent::SetInventory_UW(UTDS_InventoryMenu* UW)
{
	InventoryMenu_UW = UW;
	if (InventoryMenu_UW)
	{
		InventoryMenu_UW->CreateSlots(Content);
		if (SelectedItemIndex >= 0 && SelectedItemIndex < Content.Num())
		{
			if (!Content[SelectedItemIndex].bEmpty)
				InventoryMenu_UW->SetSelectedIndex(SelectedItemIndex);
		}
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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("bEmpty: %d\nItem number: %d -> %s"),
				Slot.bEmpty, Counter, *Slot.ItemData.ItemName.ToString()));
		}
		Counter++;
	}
}

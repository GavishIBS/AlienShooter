// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_InventoryMenu.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "UI/Widgets/TDS_InventorySlot.h"

UTDS_InventoryMenu::UTDS_InventoryMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_InventoryMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDS_InventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTDS_InventoryMenu::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_InventoryMenu::InitComponent()
{
}

void UTDS_InventoryMenu::OnNewSlotSelected(const int OldIndex, const int NewIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("OldIndex: %d\nNewIndex: %d\n"), OldIndex, NewIndex));
	if (OldIndex >= 0 && OldIndex < ItemSlots.Num())
	{
		if (IsValid(ItemSlots[OldIndex]))
		{
			ItemSlots[OldIndex]->SetSelected(false);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("UnselectedIndex: %d\n"), OldIndex));
		}
	}
	if (NewIndex >= 0 && NewIndex < ItemSlots.Num())
	{
		if (IsValid(ItemSlots[NewIndex]))
		{
			ItemSlots[NewIndex]->SetSelected(true);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("SelectedIndex: %d\n"), NewIndex));
		}
	}
}

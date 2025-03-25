// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_EquipmentShopWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "UI/Widgets/TDS_EquipmentShop_Slot.h"

#include "Game/MainGameInstance.h"

#include "Components/CombatComponent.h"
#include "Components/InventoryComponent.h"

UTDS_EquipmentShopWidget::UTDS_EquipmentShopWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_EquipmentShopWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDS_EquipmentShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CreateItemSlots();
}

void UTDS_EquipmentShopWidget::NativeDestruct()
{
	Super::NativeDestruct();
	/*
	TArray<TSubclassOf<AActor>> OutKeys;
	TArray<bool> OutValues;
	PurchasedItems.GetKeys(OutKeys);
	PurchasedItems.GenerateValueArray(OutValues);
	*/
	for (int i = 0; i < PurchasedItems.Num(); i++)
	{

		if (GetOwningPlayer() && GetOwningPlayer()->GetPawn())
		{
			if (PurchasedItems[i].bIsWeapon == true)
			{
				if (UCombatComponent* CombatC = GetOwningPlayer()->GetPawn()->GetComponentByClass<UCombatComponent>()) {
					CombatC->AddWeaponFromClass(PurchasedItems[i].ItemClass);
				}
			}
			else {
				if (UInventoryComponent* InventoryC = GetOwningPlayer()->GetPawn()->GetComponentByClass<UInventoryComponent>()) {
					InventoryC->AddItemToInventoryFromClass(PurchasedItems[i].ItemClass);
				}
			}
		}
	}
}

void UTDS_EquipmentShopWidget::CreateItemSlots()
{
	if (InventoryItems_HorizontalBox == nullptr || Weapons_HorizontalBox == nullptr) return;

	InventoryItems_HorizontalBox->ClearChildren();
	for (int i = 0; i < EquipmentDataToSell.InventoryItemsToSell.Num(); i++)
	{
		UTDS_EquipmentShop_Slot* NewSlot = CreateInventoryItemSlot(i);
		if (!NewSlot) break;
		InventoryItems_HorizontalBox->AddChild((UUserWidget*)NewSlot);
		NewSlot->SetMainGameInstanceRef(MainGameInst);
		NewSlot->OnItemHover.AddDynamic(this, &UTDS_EquipmentShopWidget::SetItemInfo);
		NewSlot->OnItemPurchased.AddDynamic(this, &UTDS_EquipmentShopWidget::OnBuyItem);
	}

	Weapons_HorizontalBox->ClearChildren();
	for (int i = 0; i < EquipmentDataToSell.WeaponsToSell.Num(); i++)
	{
		UTDS_EquipmentShop_Slot* NewSlot = CreateWeaponSlot(i);
		if (!NewSlot) break;
		Weapons_HorizontalBox->AddChild((UUserWidget*)NewSlot);
		NewSlot->SetMainGameInstanceRef(MainGameInst);
		NewSlot->OnItemHover.AddDynamic(this, &UTDS_EquipmentShopWidget::SetItemInfo);
		NewSlot->OnItemPurchased.AddDynamic(this, &UTDS_EquipmentShopWidget::OnBuyItem);
	}
}

void UTDS_EquipmentShopWidget::OnBuyItem(int Price, TSubclassOf<AActor> ItemClass, bool bIsWeapon)
{
	MainGameInst->DeductCash(Price);
	AvailableCash -= Price;
	PurchasedItems.Add({ItemClass, bIsWeapon});
	GEngine->AddOnScreenDebugMessage(-1, 34.f, FColor::Cyan, FString("BUY ITEM END"));
}

void UTDS_EquipmentShopWidget::SetItemInfo(FText InItemInfoText)
{
	CurrentItemInfoText = InItemInfoText;
}

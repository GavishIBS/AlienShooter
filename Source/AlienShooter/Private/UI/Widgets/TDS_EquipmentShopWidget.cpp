// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_EquipmentShopWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "UI/Widgets/TDS_EquipmentShop_Slot.h"

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
		NewSlot->OnItemHover.AddDynamic(this, &UTDS_EquipmentShopWidget::SetItemInfo);
	}

	Weapons_HorizontalBox->ClearChildren();
	for (int i = 0; i < EquipmentDataToSell.WeaponsToSell.Num(); i++)
	{
		UTDS_EquipmentShop_Slot* NewSlot = CreateWeaponSlot(i);
		if (!NewSlot) break;
		Weapons_HorizontalBox->AddChild((UUserWidget*)NewSlot);
		NewSlot->OnItemHover.AddDynamic(this, &UTDS_EquipmentShopWidget::SetItemInfo);
	}
}

void UTDS_EquipmentShopWidget::SetItemInfo(FText InItemInfoText)
{
	CurrentItemInfoText = InItemInfoText;
}

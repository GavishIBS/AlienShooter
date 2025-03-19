// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_EquipmentShopWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

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
		UUserWidget* NewSlot = CreateInventoryItemSlot(i);
		if (!NewSlot) break;
		InventoryItems_HorizontalBox->AddChild(NewSlot);
	}

	Weapons_HorizontalBox->ClearChildren();
	for (int i = 0; i < EquipmentDataToSell.WeaponsToSell.Num(); i++)
	{
		UUserWidget* NewSlot = CreateWeaponSlot(i);
		if (!NewSlot) break;
		Weapons_HorizontalBox->AddChild(NewSlot);
	}
}


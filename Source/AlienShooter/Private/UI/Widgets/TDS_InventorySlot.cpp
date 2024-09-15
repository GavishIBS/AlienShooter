// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_InventorySlot.h"
#include "Components/SizeBox.h"

UTDS_InventorySlot::UTDS_InventorySlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_InventorySlot::NativePreConstruct()
{
}

void UTDS_InventorySlot::NativeConstruct()
{
	InitComponent();
}

void UTDS_InventorySlot::NativeDestruct()
{
}

void UTDS_InventorySlot::InitComponent()
{
}

void UTDS_InventorySlot::SetSlotSize(FVector2D Dimensions)
{
	if (ItemBox)
	{
		ItemBox->SetWidthOverride(Dimensions.X);
		ItemBox->SetHeightOverride(Dimensions.Y);
	}
}

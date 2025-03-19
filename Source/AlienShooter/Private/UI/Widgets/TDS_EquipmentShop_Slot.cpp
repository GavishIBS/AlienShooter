// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_EquipmentShop_Slot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

UTDS_EquipmentShop_Slot::UTDS_EquipmentShop_Slot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_EquipmentShop_Slot::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDS_EquipmentShop_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	BindCallbacks();
	ItemName_Text->SetText(FText::FromString(ItemName.ToString()));
	Price_Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), Price)));
	if (Icon) {
		Icon_Image->SetBrushFromTexture(Icon, true);
	}
}

void UTDS_EquipmentShop_Slot::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_EquipmentShop_Slot::BindCallbacks()
{
	if (WholeButton) {
		WholeButton->OnHovered.AddDynamic(this, &UTDS_EquipmentShop_Slot::HoverCallback);
	}
	if (BuyButton) {
		BuyButton->OnClicked.AddDynamic(this, &UTDS_EquipmentShop_Slot::BuyButtonCallback);
	}
}

void UTDS_EquipmentShop_Slot::HoverCallback()
{
	OnItemHover.Broadcast(ItemInfoText);
}

void UTDS_EquipmentShop_Slot::BuyButtonCallback()
{
}

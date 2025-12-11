// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_KeySmashUserWidget.h"
#include "Components/Image.h"

UTDS_KeySmashUserWidget::UTDS_KeySmashUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_KeySmashUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetIsFocusable(true);
}

void UTDS_KeySmashUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindCallbacks();
}

void UTDS_KeySmashUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_KeySmashUserWidget::BindCallbacks()
{
}

void UTDS_KeySmashUserWidget::SetKeySmashProperties(const FKeySmashProperties& KeySmashProperties)
{
	if (KeySmashProperties.KeyGroup_Icon) {
		KeyGroup_Icon->SetBrushFromTexture(KeySmashProperties.KeyGroup_Icon, bShouldIconMatchSize);
	}

	OnKeySmashPropertiesUpdated.Broadcast(KeySmashProperties);
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_DialogueLogsUserWidget.h"

UTDS_DialogueLogsUserWidget::UTDS_DialogueLogsUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_DialogueLogsUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDS_DialogueLogsUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindCallbacks();
}

void UTDS_DialogueLogsUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_DialogueLogsUserWidget::BindCallbacks()
{
}

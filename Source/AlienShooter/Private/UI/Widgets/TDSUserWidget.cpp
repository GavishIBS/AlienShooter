// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#include "UI/Widgets/TDSUserWidget.h"

UTDSUserWidget::UTDSUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDSUserWidget::SetOwnerCharacter(AShooterCharacter* InCharacter)
{
	OwnerAsShooterCharacter = InCharacter;
}

void UTDSUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDSUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTDSUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDSUserWidget::InitComponent()
{
}

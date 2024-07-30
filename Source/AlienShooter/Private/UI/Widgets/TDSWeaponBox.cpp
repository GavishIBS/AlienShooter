// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDSWeaponBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

UTDSWeaponBox::UTDSWeaponBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDSWeaponBox::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDSWeaponBox::NativeConstruct()
{
	Super::NativeConstruct();
	InitComponent();
}

void UTDSWeaponBox::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDSWeaponBox::InitComponent()
{
	Super::InitComponent();

	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(RootSize.X);
		SizeBox_Root->SetHeightOverride(RootSize.Y);
	}
}

void UTDSWeaponBox::InitBox()
{
	if (WeaponIcon && AmmoText)
	{
		WeaponIcon->SetRenderOpacity(1.f);
		SizeBox_AmmoBox->SetRenderOpacity(1.f);
		WeaponIcon->SetBrushFromTexture(Icon);
		AmmoText->SetText(FText::AsNumber(Ammo));
	}
}

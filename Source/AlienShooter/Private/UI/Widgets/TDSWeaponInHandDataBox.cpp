// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDSWeaponInHandDataBox.h"
#include "Characters/ShooterCharacter.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

UTDSWeaponInHandDataBox::UTDSWeaponInHandDataBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDSWeaponInHandDataBox::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(RootSize.X);
		SizeBox_Root->SetHeightOverride(RootSize.Y);
	}
}

void UTDSWeaponInHandDataBox::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTDSWeaponInHandDataBox::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDSWeaponInHandDataBox::InitComponent()
{
	Super::InitComponent();

	if (OwnerAsShooterCharacter && OwnerAsShooterCharacter->GetCombatComponent())
	{
		OwnerAsShooterCharacter->GetCombatComponent()->OnWeaponUpdated.AddDynamic(this, &UTDSWeaponInHandDataBox::OnWeaponUpdatedCallback);
	}
}

void UTDSWeaponInHandDataBox::OnWeaponUpdatedCallback()
{
	if (!OwnerAsShooterCharacter) return;

	if (WeaponIcon && AmmoText)
	{
		if (OwnerAsShooterCharacter->GetWeaponInHand())
		{
			WeaponIcon->SetRenderOpacity(1.f);
			SizeBox_AmmoBox->SetRenderOpacity(1.f);
			WeaponIcon->SetBrushFromTexture(OwnerAsShooterCharacter->GetWeaponInHand()->GetWeaponProperties().WeaponIcon);
			AmmoText->SetText(FText::AsNumber(OwnerAsShooterCharacter->GetWeaponInHand()->GetTotalAmmoAvailable()));
		} else
		{
			WeaponIcon->SetRenderOpacity(0.f);
			SizeBox_AmmoBox->SetRenderOpacity(0.f);
		}
	}
}

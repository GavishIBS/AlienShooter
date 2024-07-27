// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDSOverlay.h"
#include "UI/Widgets/TDSWeaponInHandDataBox.h"

UTDSOverlay::UTDSOverlay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDSOverlay::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (WeaponInHandDataBox)
		WeaponInHandDataBox->SetDesiredSizeInViewport(WeaponInHandDataBox->GetRootSize());
}

void UTDSOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	InitComponents();
	GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Yellow,FString("jhsdfb"));
}

void UTDSOverlay::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDSOverlay::InitComponents()
{
	if (WeaponInHandDataBox)
	{
		WeaponInHandDataBox->SetOwnerCharacter(OwnerAsShooterCharacter);
		WeaponInHandDataBox->InitComponent();
	}
}

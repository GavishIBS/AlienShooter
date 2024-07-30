// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDSWeaponBoxList.h"

#include "Characters/ShooterCharacter.h"
#include "Components/VerticalBox.h"
#include "UI/Widgets/TDSWeaponBox.h"

UTDSWeaponBoxList::UTDSWeaponBoxList(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDSWeaponBoxList::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDSWeaponBoxList::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTDSWeaponBoxList::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDSWeaponBoxList::InitComponent()
{
	Super::InitComponent();

	if (OwnerAsShooterCharacter && OwnerAsShooterCharacter->GetCombatComponent())
	{
		OwnerAsShooterCharacter->GetCombatComponent()->OnNewWeaponAdded.AddDynamic(this, &UTDSWeaponBoxList::OnNewWeaponAddedCallback);
		OwnerAsShooterCharacter->GetCombatComponent()->OnWeaponUpdated.AddDynamic(this, &UTDSWeaponBoxList::OnWeaponUpdatedCallback);
	}
}

void UTDSWeaponBoxList::OnNewWeaponAddedCallback(const AWeaponBase* InWeapon)
{
	WeaponIcons.Add(InWeapon->GetWeaponProperties()->WeaponIcon);
}

void UTDSWeaponBoxList::OnWeaponUpdatedCallback()
{
	if (!OwnerAsShooterCharacter || !WeaponBoxClass) return;
	
	VerticalBox_List->ClearChildren();

	APlayerController* PlayerController = Cast<APlayerController>(OwnerAsShooterCharacter->GetController());
	if (!PlayerController) return;

	for (auto& WeaponIcon : WeaponIcons)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString("sffghsfdh"));
		UWidget* WeaponBoxAsWidget = CreateWidget(PlayerController, WeaponBoxClass);
		UTDSWeaponBox* WeaponBox = Cast<UTDSWeaponBox>(WeaponBoxAsWidget);
		WeaponBox->SetIcon(WeaponIcon);
		WeaponBox->InitBox();
		VerticalBox_List->AddChild(WeaponBox);
	}
}

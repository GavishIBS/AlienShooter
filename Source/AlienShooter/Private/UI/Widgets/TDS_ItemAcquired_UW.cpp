// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "UI/Widgets/TDS_ItemAcquired_UW.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Characters/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

UTDS_ItemAcquired_UW::UTDS_ItemAcquired_UW(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTDS_ItemAcquired_UW::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTDS_ItemAcquired_UW::NativeConstruct()
{
	Super::NativeConstruct();
	BindCallbacks();

	if (Item_Icon) {
		Item_Icon->SetBrushFromTexture(Item_Icon_Texture, true);
	}
	if (ItemDesc_Text) {
		ItemDesc_Text->SetText(FText::FromString(ItemDesc));
	}
}

void UTDS_ItemAcquired_UW::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTDS_ItemAcquired_UW::BindCallbacks()
{
	if (OK_Button) {
		OK_Button->OnClicked.AddDynamic(this, &UTDS_ItemAcquired_UW::OnOKButtonClicked);
	}
}

void UTDS_ItemAcquired_UW::OnOKButtonClicked()
{
	if (TObjectPtr<APlayerController> PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		PC->SetInputMode(InputMode);

		UGameplayStatics::SetGamePaused(this, false);

		RemoveFromParent();
	}
}


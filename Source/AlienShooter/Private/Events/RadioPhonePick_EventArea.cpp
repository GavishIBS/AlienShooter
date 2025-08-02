// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Events/RadioPhonePick_EventArea.h"
#include "Components/RadioPhoneComponent.h"

#include "Kismet/GameplayStatics.h"

ARadioPhonePick_EventArea::ARadioPhonePick_EventArea()
{

}

void ARadioPhonePick_EventArea::BeginPlay()
{
	Super::BeginPlay();
}

void ARadioPhonePick_EventArea::BindCallbacks()
{
	Super::BindCallbacks();
	OnEnterArea.AddDynamic(this, &ThisClass::TryPickRadioPhone);
}

void ARadioPhonePick_EventArea::TryPickRadioPhone(AActor* OtherActor)
{
	if (!OtherActor) return;
	if (TObjectPtr<URadioPhoneComponent> RP_Comp = OtherActor->GetComponentByClass<URadioPhoneComponent>()) {
		RP_Comp->OnPhonePicked();
		
		if (TObjectPtr<APlayerController> PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			UTDS_ItemAcquired_UW* UW = CreateItemAquiredWidget(ItemAquiredUW_Class);
			if (UW != nullptr) {
				FInputModeGameAndUI InputMode;
				InputMode.SetHideCursorDuringCapture(false);
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(InputMode);
				
				UGameplayStatics::SetGamePaused(this, true);

				Destroy();
			}
		}
	}
}

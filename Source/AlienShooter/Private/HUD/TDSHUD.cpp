// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "HUD/TDSHUD.h"
#include "Blueprint/UserWidget.h"
#include "Characters/ShooterCharacter.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "UI/Widgets/TDSOverlay.h"

ATDSHUD::ATDSHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATDSHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		PlayerController = World->GetFirstPlayerController();
		if (PlayerController && OverlayWidgetClass)
			OverlayWidget = Cast<UTDSUserWidget>(CreateWidget(PlayerController, OverlayWidgetClass));
		if (OverlayWidget)
		{
			OverlayWidget->SetOwnerCharacter(Cast<AShooterCharacter>(PlayerController->GetPawn()));
			OverlayWidget->AddToViewport();
		}
	}
}

UTDSUserWidget* ATDSHUD::MakeWidgetInViewport(const TSubclassOf<UTDSUserWidget>& UW_Class)
{
	UTDSUserWidget* UW = CreateWidget<UTDSUserWidget>(PlayerController, UW_Class);
	UW->AddToViewport();
	return UW;
}

TObjectPtr<UTDSOverlay> ATDSHUD::GetOverlayWidget() const
{
	return Cast<UTDSOverlay>(OverlayWidget);
}

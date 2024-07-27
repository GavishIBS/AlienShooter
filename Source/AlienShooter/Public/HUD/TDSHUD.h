// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDSHUD.generated.h"

class UUserWidget;
class UTDSUserWidget;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ATDSHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATDSHUD();

protected:
	virtual void BeginPlay() override;

	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(EditDefaultsOnly, Category=Overlay)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	TObjectPtr<UTDSUserWidget> OverlayWidget;
};

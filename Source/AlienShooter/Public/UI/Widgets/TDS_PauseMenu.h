// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_PauseMenu : public UTDSUserWidget
{
	GENERATED_BODY()
	
public:
	UTDS_PauseMenu() = default;
	UTDS_PauseMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void BindCallbacks();
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_EquipmentShop_Slot.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_EquipmentShop_Slot : public UTDSUserWidget
{
	GENERATED_BODY()
	
public:
	UTDS_EquipmentShop_Slot() = default;
	UTDS_EquipmentShop_Slot(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};

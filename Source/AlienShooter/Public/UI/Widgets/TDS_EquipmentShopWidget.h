// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_EquipmentShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_EquipmentShopWidget : public UTDSUserWidget
{
	GENERATED_BODY()
	
	UTDS_EquipmentShopWidget() = default;
	UTDS_EquipmentShopWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	// Data
	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FName PlayerName;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	int AvailableCash = 0.f;
};

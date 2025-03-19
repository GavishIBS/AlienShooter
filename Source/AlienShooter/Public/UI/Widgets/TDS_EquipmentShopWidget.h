// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "../../EquipmentShopTypes.h"
#include "TDS_EquipmentShopWidget.generated.h"

class UTextBlock;
class UButton;
class UHorizontalBox;

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

	void CreateItemSlots();
	
	UFUNCTION(BlueprintImplementableEvent)
	UUserWidget* CreateInventoryItemSlot(const int& Index);
	
	UFUNCTION(BlueprintImplementableEvent)
	UUserWidget* CreateWeaponSlot(const int& Index);

	// Components
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* InventoryItems_HorizontalBox;
	
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* Weapons_HorizontalBox;

protected:
	// Data
	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FName PlayerName;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	int AvailableCash = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FEquipmentDataToSell EquipmentDataToSell = FEquipmentDataToSell();
};

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
class UTDS_EquipmentShop_Slot;
class UMainGameInstance;

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
	UTDS_EquipmentShop_Slot* CreateInventoryItemSlot(const int& Index);
	
	UFUNCTION(BlueprintImplementableEvent)
	UTDS_EquipmentShop_Slot* CreateWeaponSlot(const int& Index);

	UFUNCTION()
	void OnBuyItem(int Price, TSubclassOf<AActor> ItemClass, bool bIsWeapon);

	UFUNCTION()
	void SetItemInfo(FText InItemInfoText);

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
	UMainGameInstance* MainGameInst;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	int AvailableCash = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FEquipmentDataToSell EquipmentDataToSell = FEquipmentDataToSell();

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FText CurrentItemInfoText = FText();
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_EquipmentShop_Slot.generated.h"

class UTextBlock;
class UButton;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemHover, FText, ItemInfo);

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

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnItemHover OnItemHover;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void BindCallbacks();

	UFUNCTION()
	void HoverCallback();

	UFUNCTION()
	void BuyButtonCallback();

	// Components
	UPROPERTY(meta = (BindWidget))
	UButton* WholeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BuyButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Price_Text;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName_Text;

	UPROPERTY(meta = (BindWidget))
	UImage* Icon_Image;

protected:
	// Data
	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FName ItemName;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	int Price = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	UTexture2D* Icon = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Data", meta = (ExposeOnSpawn = "true"))
	FText ItemInfoText = FText();
};

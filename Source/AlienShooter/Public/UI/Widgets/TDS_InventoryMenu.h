// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_InventoryMenu.generated.h"

class UInventoryComponent;
class UHorizontalBox;
class USizeBox;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_InventoryMenu : public UTDSUserWidget
{
	GENERATED_BODY()
	
public:
	friend UInventoryComponent;
	UTDS_InventoryMenu() = default;
	UTDS_InventoryMenu(const FObjectInitializer& ObjectInitializer);

	virtual void InitComponent() override;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateSlots(const TArray<FSlotData>& Content);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USizeBox* SizeBox_Root;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UHorizontalBox* SlotBox;

protected:
	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<UTDSUserWidget> InventorySlow_UW_Class = nullptr;
};

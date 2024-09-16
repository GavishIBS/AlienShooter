// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_InventorySlot.generated.h"

class USizeBox;
class UImage;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_InventorySlot : public UTDSUserWidget
{
	GENERATED_BODY()
	
public:
	UTDS_InventorySlot() = default;
	UTDS_InventorySlot(const FObjectInitializer& ObjectInitializer);

	virtual void InitComponent() override;

	UFUNCTION(BlueprintCallable)
	void SetSlotSize(FVector2D Dimensions);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USizeBox* ItemBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadOnly, Category="Properties")
	bool bSelected = false;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnSlotSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlotUnselected();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSelected(const bool Value)
	{
		bSelected = Value;
		if (bSelected)
		{
			OnSlotSelected();
		}
		else OnSlotUnselected();
	}
};

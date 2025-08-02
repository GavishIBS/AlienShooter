// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDS_ItemAcquired_UW.generated.h"

class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_ItemAcquired_UW : public UTDSUserWidget
{
	GENERATED_BODY()

	UTDS_ItemAcquired_UW() = default;
	UTDS_ItemAcquired_UW(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void BindCallbacks();

	UFUNCTION()
	void OnOKButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Item_Icon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDesc_Text;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OK_Button;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UTexture2D> Item_Icon_Texture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FString ItemDesc;
};

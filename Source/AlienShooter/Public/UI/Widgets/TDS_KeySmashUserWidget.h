// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "UI_Types.h"
#include "TDS_KeySmashUserWidget.generated.h"

class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeySmashPropertiesUpdated, FKeySmashProperties, KeySmashProperties);

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDS_KeySmashUserWidget : public UTDSUserWidget
{
	GENERATED_BODY()

public:
	UTDS_KeySmashUserWidget() = default;
	UTDS_KeySmashUserWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FOnKeySmashPropertiesUpdated OnKeySmashPropertiesUpdated;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void BindCallbacks();

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> KeyGroup_Icon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> DescText;

	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bShouldIconMatchSize = false;

public:
	void SetKeySmashProperties(const FKeySmashProperties& KeySmashProperties);
};

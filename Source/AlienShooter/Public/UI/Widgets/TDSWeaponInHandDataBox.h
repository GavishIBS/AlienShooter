// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDSWeaponInHandDataBox.generated.h"

class UTextBlock;
class UImage;
class USizeBox;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDSWeaponInHandDataBox : public UTDSUserWidget
{
	GENERATED_BODY()

public:
	UTDSWeaponInHandDataBox() = default;
	UTDSWeaponInHandDataBox(const FObjectInitializer& ObjectInitializer);

	virtual void InitComponent() override;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnWeaponUpdatedCallback();

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> WeaponIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_AmmoBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AmmoText;

protected:
	// Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FVector2D RootSize = FVector2D();

public:
	FORCEINLINE FVector2d GetRootSize() const {return RootSize;}
};

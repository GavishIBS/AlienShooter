// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDSWeaponBoxList.generated.h"

class USizeBox;
class UVerticalBox;
class UCanvasPanel;
class UTDSWeaponBox;
class AWeaponBase;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDSWeaponBoxList : public UTDSUserWidget
{
	GENERATED_BODY()

public:
	UTDSWeaponBoxList() = default;
	UTDSWeaponBoxList(const FObjectInitializer& ObjectInitializer);

	virtual void InitComponent() override;
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnNewWeaponAddedCallback(const AWeaponBase* InWeapon);

	UFUNCTION()
	void OnWeaponUpdatedCallback();

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_List;

	TArray<UTDSWeaponBox> WeaponBoxes;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	TSubclassOf<UTDSWeaponBox> WeaponBoxClass;

	TArray<TObjectPtr<UTexture2D>> WeaponIcons;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDSWeaponBox.generated.h"

class USizeBox;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDSWeaponBox : public UTDSUserWidget
{
	GENERATED_BODY()

public:
	UTDSWeaponBox() = default;
	UTDSWeaponBox(const FObjectInitializer& ObjectInitializer);

	virtual void InitComponent() override;

	void InitBox();
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> WeaponIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_AmmoBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AmmoText;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	FVector2D RootSize = FVector2D();

	UPROPERTY(VisibleDefaultsOnly, Category=Properties)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(VisibleDefaultsOnly, Category=Properties)
	int Ammo;

public:
	FORCEINLINE void SetIcon(const TObjectPtr<UTexture2D>& Value) {Icon = Value;}
	FORCEINLINE void SetAmmo(const int& Value) {Ammo = Value;}
};

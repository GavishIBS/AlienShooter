// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TDSUserWidget.h"
#include "TDSOverlay.generated.h"

class UTDSWeaponBoxList;
class AShooterCharacter;
class UTDSWeaponInHandDataBox;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDSOverlay : public UTDSUserWidget
{
	GENERATED_BODY()

public:
	UTDSOverlay() = default;
	UTDSOverlay(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void InitComponents();
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTDSWeaponInHandDataBox> WeaponInHandDataBox;

	//UPROPERTY(meta=(BindWidget))
	//TObjectPtr<UTDSWeaponBoxList> WeaponBoxList;
};

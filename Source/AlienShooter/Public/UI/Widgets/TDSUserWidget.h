// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDSUserWidget.generated.h"

class AShooterCharacter;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UTDSUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTDSUserWidget() = default;
	UTDSUserWidget(const FObjectInitializer& ObjectInitializer);

	void SetOwnerCharacter(AShooterCharacter* InCharacter);
	virtual void InitComponent();

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, Category=Ownership)
	TObjectPtr<AShooterCharacter> OwnerAsShooterCharacter;
};

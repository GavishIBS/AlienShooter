// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Events/EventArea.h"
#include "RadioPhonePick_EventArea.generated.h"

class UTDS_ItemAcquired_UW;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ARadioPhonePick_EventArea : public AEventArea
{
	GENERATED_BODY()

public:
	ARadioPhonePick_EventArea();

protected:
	virtual void BeginPlay() override;
	virtual void BindCallbacks() override;

	UFUNCTION()
	void TryPickRadioPhone(AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent)
	UTDS_ItemAcquired_UW* CreateItemAquiredWidget(TSubclassOf<UTDS_ItemAcquired_UW> InClass);

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<UTDS_ItemAcquired_UW> ItemAquiredUW_Class;
};

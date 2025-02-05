// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RadioPhoneComponent.generated.h"

class USoundBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API URadioPhoneComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:	
	URadioPhoneComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnPhonePicked();

	UFUNCTION(BlueprintCallable)
	bool TriggerAudio(USoundBase* InAudio);

	void EndAudio();

	bool bHavePhone = false;
	bool bAudioInProgress = false;
	
	FTimerHandle AudioTimerHandle;
};

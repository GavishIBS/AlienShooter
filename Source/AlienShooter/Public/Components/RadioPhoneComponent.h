// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "../CustomAudioTypes.h"
#include "RadioPhoneComponent.generated.h"

class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAudioTriggered, const EAudioType, AudioType, const FString, AudioInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API URadioPhoneComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:	
	URadioPhoneComponent();

	UPROPERTY(BlueprintAssignable)
	FOnAudioTriggered OnAudioTriggered;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnPhonePicked();

	UFUNCTION(BlueprintCallable)
	bool TriggerAudio(USoundBase* InAudio, const EAudioType AudioType);

	void EndAudio();

	bool bHavePhone = false;
	bool bAudioInProgress = false;
	
	FTimerHandle AudioTimerHandle;
};

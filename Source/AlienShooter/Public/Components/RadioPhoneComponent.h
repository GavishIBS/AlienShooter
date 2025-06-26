// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "../CustomAudioTypes.h"
#include "RadioPhoneComponent.generated.h"

class USoundBase;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAudioTriggered, const EAudioType, AudioType, const FString, AudioInfo, const float, AudioDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAudioCompleted);
// FOnAudioCompleted - triggered after the whole audio is ended that is including response audio

USTRUCT(BlueprintType)
struct FOnAudioTriggerData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data, meta=(ClampMin="0", ClampMax="1000"))
	float MaxWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data)
	TObjectPtr<UAnimMontage> ToggleRadioPhone_AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data)
	FName AttachmentSocketName = FName();
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API URadioPhoneComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:	
	URadioPhoneComponent();

	UPROPERTY(BlueprintAssignable)
	FOnAudioTriggered OnAudioTriggered;
	UPROPERTY(BlueprintAssignable)
	FOnAudioCompleted OnAudioCompleted;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnPhonePicked();

	UFUNCTION(BlueprintCallable)
	bool TriggerAudio(USoundBase* InAudio, const EAudioType AudioType);
	
	UFUNCTION(BlueprintCallable)
	bool TriggerAudio_WithParams(const FAudioEvent AudioEvent);
	
	UFUNCTION(BlueprintCallable)
	bool TriggerAudio_WithParams_Response(const FAudioEvent AudioEvent, const FAudioEvent ResponseAudioEvent, float ResponseDelay = 3);

	void EndAudio();

	bool bHavePhone = false;
	bool bAudioInProgress = false;
	
	FTimerHandle AudioTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	FOnAudioTriggerData OnAudioStartedData;
	
	UPROPERTY(EditDefaultsOnly, Category=Properties)
	FOnAudioTriggerData OnAudioCompletedData;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HaveRadioPhone() const { return bHavePhone; }

	FORCEINLINE bool IsAudioInProgress() const { return bAudioInProgress; }

	FORCEINLINE float GetOnAudioStarted_MaxWalkSpeed() const { return OnAudioStartedData.MaxWalkSpeed; }
	FORCEINLINE float GetOnAudioCompleted_MaxWalkSpeed() const { return OnAudioCompletedData.MaxWalkSpeed; }

	FORCEINLINE TObjectPtr<UAnimMontage> GetOnAudioStarted_ToggleRadioPhone_AnimMontage() const { return OnAudioStartedData.ToggleRadioPhone_AnimMontage; }
	FORCEINLINE TObjectPtr<UAnimMontage> GetOnAudioCompleted_ToggleRadioPhone_AnimMontage() const { return OnAudioCompletedData.ToggleRadioPhone_AnimMontage; }
	
	FORCEINLINE FName GetInHandAttachment_SocketName() const { return OnAudioStartedData.AttachmentSocketName; }
	FORCEINLINE FName GetOutHandAttachment_SocketName() const { return OnAudioCompletedData.AttachmentSocketName; }
};

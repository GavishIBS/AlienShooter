// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "../CustomAudioTypes.h"
#include "RadioPhoneComponent.generated.h"

class USoundBase;
class UAnimMontage;
class UObjectiveComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAudioTriggered, const EAudioType, AudioType, const FString, AudioInfo, const float, AudioDuration, const FName, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAudioCompleted);
// FOnAudioCompleted - triggered after the whole audio is ended that is including response audio
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIncomingMessage, bool, bRead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlinkToggle, bool, bBlink);

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
	friend class ABaseCharacter;
	URadioPhoneComponent();

	UPROPERTY(BlueprintAssignable)
	FOnAudioTriggered OnAudioTriggered;
	
	UPROPERTY(BlueprintAssignable)
	FOnAudioCompleted OnAudioCompleted;
	
	UPROPERTY(BlueprintAssignable)
	FOnIncomingMessage OnIncomingMessage;
	
	UPROPERTY(BlueprintAssignable)
	FOnBlinkToggle OnBlinkToggle;

protected:
	virtual void BeginPlay() override;

	// Objectives begin
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TWeakObjectPtr<UObjectiveComponent> ObjectiveComponent;

	void SetObjectiveComponent(TObjectPtr<UObjectiveComponent> Component);

	UFUNCTION()
	void AddDialogueLog(const EAudioType AudioType, const FString AudioInfo, const float AudioDuration, const FName Instigator);
public:
	UFUNCTION(BlueprintCallable)  // TODO: TEMP
	UObjectiveComponent* GetObjectiveComponent() const { return ObjectiveComponent.Get(); }

	UFUNCTION(BlueprintCallable)  // TODO: TEMP
	bool RemindObjective(FDataTableRowHandle ObjectiveID);
	UFUNCTION(BlueprintCallable)  // TODO: TEMP
	bool RemindObjective_WithKey(const int32 Key);
	// Objectives end

	UFUNCTION(BlueprintCallable)
	void OnPhonePicked();

	UFUNCTION(BlueprintCallable)
	bool TriggerAudio(USoundBase* InAudio, const EAudioType AudioType);
	
	UFUNCTION(BlueprintCallable)
	bool TriggerAudio_WithParams(const FAudioEvent AudioEvent);
	
	UFUNCTION(BlueprintCallable)
	bool TriggerAudio_WithParams_Response(const FAudioEvent AudioEvent, const FAudioEvent ResponseAudioEvent, float ResponseDelay = 3);
	
	UFUNCTION(BlueprintCallable)
	bool AddIncomingMessage(const FAudioEvent AudioEvent);
	
	UFUNCTION(BlueprintCallable)
	bool TriggerIncomingMessage();

	void EndAudio();

	void StartBlinkTimer();
	void EndBlinkTimer();

	bool bHavePhone = false;
	bool bAudioInProgress = false;

	bool bIncomingMessage = false;
	FAudioEvent IncomingMessage;
	//TArray<const FAudioEvent> IncomingMessages; // TODO
	
	bool bBlink_ThisFrame = false;
	UPROPERTY(EditAnywhere, Category=Properties)
	float BlinkRate = 0.1f;

	FTimerHandle AudioTimerHandle;
	FTimerHandle BlinkTimer;

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

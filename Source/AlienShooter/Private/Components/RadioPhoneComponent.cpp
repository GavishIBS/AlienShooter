// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/RadioPhoneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Electricity/Actors/ElectricPointLight.h"
#include "Components/ObjectiveComponent.h"

URadioPhoneComponent::URadioPhoneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetVisibility(false);
}

void URadioPhoneComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URadioPhoneComponent::SetObjectiveComponent(TObjectPtr<UObjectiveComponent> Component)
{
	ObjectiveComponent = Component.Get();
	OnAudioTriggered.AddDynamic(this, &URadioPhoneComponent::AddDialogueLog);
}

void URadioPhoneComponent::AddDialogueLog(const EAudioType AudioType, const FString AudioInfo, const float AudioDuration, const FName Instigator)
{
	if (ObjectiveComponent != nullptr) {
		FDialogueLog DialogueLog{ Instigator, AudioInfo };
		ObjectiveComponent->AddRadioPhoneDialogueLog(std::move(DialogueLog));
	}
}

bool URadioPhoneComponent::RemindObjective(FDataTableRowHandle ObjectiveID)
{
	if (ObjectiveComponent == nullptr) return false;
	
	FRemindableObjective* RemindableObj = ObjectiveID.GetRow<FRemindableObjective>(TEXT(""));
	if (RemindableObj == nullptr) return false;
	
	return RemindObjective_WithKey(RemindableObj->ObjectiveData.Key);
}

bool URadioPhoneComponent::RemindObjective_WithKey(const int32 Key)
{
	if (ObjectiveComponent == nullptr) return false;
	
	std::tuple<bool, FReminderInfo> ReminderData = ObjectiveComponent->TriggerReminder(Key);
	if (std::get<0>(ReminderData)) {
		const FReminderInfo ReminderInfo = std::get<1>(ReminderData);
		FAudioEvent AudioEvent = FAudioEvent(ReminderInfo.RemindingAudio, EAudioType::EAT_Dialogue, ReminderInfo.RemindingText,
			FName("Radio room operator"));
		TriggerAudio_WithParams(AudioEvent);

		return true;
	}
	
	return false;
}

void URadioPhoneComponent::OnPhonePicked()
{
	SetVisibility(true);
	bHavePhone = true;
}

bool URadioPhoneComponent::TriggerAudio(USoundBase* InAudio, const EAudioType AudioType)   // TODO: make FRadioPhoneAudio -> take in the struct as argument containing all the information
{
	if (bAudioInProgress) return false;
	if (InAudio == nullptr) return false;
	if (bHavePhone == false) return false;

	UGameplayStatics::PlaySoundAtLocation(this, InAudio, GetComponentLocation());
	bAudioInProgress = true;
	OnAudioTriggered.Broadcast(EAudioType::EAT_Dialogue, "Hello this is a sample text", InAudio->Duration, FName());

	GetOwner()->GetWorldTimerManager().SetTimer(AudioTimerHandle, this, &URadioPhoneComponent::EndAudio, InAudio->Duration, false, InAudio->Duration);

	return true;
}

bool URadioPhoneComponent::TriggerAudio_WithParams(const FAudioEvent AudioEvent)
{
	if (bAudioInProgress) return false;
	if (AudioEvent.AudioAsset == nullptr) return false;

	if (bHavePhone == false) return false;

	UGameplayStatics::PlaySoundAtLocation(this, AudioEvent.AudioAsset, GetComponentLocation());
	bAudioInProgress = true;
	OnAudioTriggered.Broadcast(EAudioType::EAT_Dialogue, AudioEvent.AudioInfo, AudioEvent.AudioAsset->Duration, AudioEvent.Instigator);

	GetOwner()->GetWorldTimerManager().SetTimer(AudioTimerHandle, this, &URadioPhoneComponent::EndAudio, AudioEvent.AudioAsset->Duration, false, AudioEvent.AudioAsset->Duration);

	return true;
}

bool URadioPhoneComponent::TriggerAudio_WithParams_Response(const FAudioEvent AudioEvent, const FAudioEvent ResponseAudioEvent, float ResponseDelay)
{
	if (bAudioInProgress) return false;
	if (AudioEvent.AudioAsset == nullptr) return false;
	if (bHavePhone == false) return false;

	UGameplayStatics::PlaySoundAtLocation(this, AudioEvent.AudioAsset, GetComponentLocation());
	bAudioInProgress = true;
	OnAudioTriggered.Broadcast(EAudioType::EAT_Dialogue, AudioEvent.AudioInfo, AudioEvent.AudioAsset->Duration, AudioEvent.Instigator);

	//GetOwner()->GetWorldTimerManager().SetTimer(AudioTimerHandle, this, &URadioPhoneComponent::EndAudio, AudioEvent.AudioAsset->Duration, false, AudioEvent.AudioAsset->Duration);
	GetWorld()->GetTimerManager().SetTimer(
		AudioTimerHandle,
		FTimerDelegate::CreateLambda([this, ResponseAudioEvent]() {
			if (ResponseAudioEvent.AudioAsset) {
				UGameplayStatics::PlaySoundAtLocation(this, ResponseAudioEvent.AudioAsset, GetComponentLocation());
				OnAudioTriggered.Broadcast(EAudioType::EAT_Dialogue, ResponseAudioEvent.AudioInfo, ResponseAudioEvent.AudioAsset->Duration, ResponseAudioEvent.Instigator);
				FTimerHandle ResponseAudioTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(ResponseAudioTimerHandle, FTimerDelegate::CreateLambda([this]() {
					bAudioInProgress = false;
					OnAudioCompleted.Broadcast();
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString("Response dialogue have been completed"));
					}), ResponseAudioEvent.AudioAsset->Duration, false, ResponseAudioEvent.AudioAsset->Duration);
			}
			else {
				EndAudio();
			}
			}),
		AudioEvent.AudioAsset->Duration, false, AudioEvent.AudioAsset->Duration + ResponseDelay
	);

	return true;
}

bool URadioPhoneComponent::AddIncomingMessage(const FAudioEvent AudioEvent)
{
	if (bIncomingMessage) return false;  // there is already an incoming message

	bIncomingMessage = true;
	IncomingMessage = AudioEvent;

	OnIncomingMessage.Broadcast(false);

	StartBlinkTimer();

	return true;
}

bool URadioPhoneComponent::TriggerIncomingMessage()
{
	if (bIncomingMessage == false || IncomingMessage.AudioAsset == nullptr) return false;
	
	bool bReturnValue = TriggerAudio_WithParams(IncomingMessage);
	IncomingMessage = FAudioEvent();


	OnIncomingMessage.Broadcast(true);

	EndBlinkTimer();

	return bReturnValue;
}

void URadioPhoneComponent::EndAudio()
{
	bAudioInProgress = false;
	OnAudioCompleted.Broadcast();
}

void URadioPhoneComponent::StartBlinkTimer()
{
	GetOwner()->GetWorldTimerManager().SetTimer(BlinkTimer, FTimerDelegate::CreateLambda([this]() {
		bBlink_ThisFrame = !bBlink_ThisFrame;
		OnBlinkToggle.Broadcast(bBlink_ThisFrame);
	}), BlinkRate, true, 0.f);
}

void URadioPhoneComponent::EndBlinkTimer()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(BlinkTimer);
}

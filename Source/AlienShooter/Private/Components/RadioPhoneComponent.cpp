// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/RadioPhoneComponent.h"
#include "Kismet/GameplayStatics.h"

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
	OnAudioTriggered.Broadcast(EAudioType::EAT_Dialogue, "Hello this is a sample text", InAudio->Duration);

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
	OnAudioTriggered.Broadcast(EAudioType::EAT_Dialogue, AudioEvent.AudioInfo, AudioEvent.AudioAsset->Duration);

	GetOwner()->GetWorldTimerManager().SetTimer(AudioTimerHandle, this, &URadioPhoneComponent::EndAudio, AudioEvent.AudioAsset->Duration, false, AudioEvent.AudioAsset->Duration);

	return true;
}

void URadioPhoneComponent::EndAudio()
{
	bAudioInProgress = false;
}

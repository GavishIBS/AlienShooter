// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/RadioPhoneComponent.h"
#include "Kismet/GameplayStatics.h"

URadioPhoneComponent::URadioPhoneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void URadioPhoneComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URadioPhoneComponent::OnPhonePicked()
{
	bHavePhone = true;
}

bool URadioPhoneComponent::TriggerAudio(USoundBase* InAudio)   // TODO: make FRadioPhoneAudio -> take in thi struct as argument
{
	if (bAudioInProgress) return false;
	if (InAudio == nullptr) return false;
	if (bHavePhone == false) return false;

	UGameplayStatics::PlaySoundAtLocation(this, InAudio, GetComponentLocation());
	bAudioInProgress = true;

	GetOwner()->GetWorldTimerManager().SetTimer(AudioTimerHandle, this, &URadioPhoneComponent::EndAudio, InAudio->Duration, false, InAudio->Duration);

	return true;
}

void URadioPhoneComponent::EndAudio()
{
	bAudioInProgress = false;
}

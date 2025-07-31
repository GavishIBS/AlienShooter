// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomAudioTypes.generated.h"

class USoundBase;

UENUM(BlueprintType)
enum class EAudioType : uint8
{
	EAT_None = 0, EAT_Noise = 1, EAT_Dialogue = 2
};

USTRUCT(BlueprintType)
struct FAudioEvent
{
	GENERATED_BODY()

	FAudioEvent() = default;
	FAudioEvent(USoundBase* InAudio, EAudioType Type, FString Info, FName InInstigator) :
		AudioAsset(InAudio), AudioType(Type), AudioInfo(Info), Instigator(InInstigator) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	USoundBase* AudioAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EAudioType AudioType = EAudioType::EAT_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString AudioInfo = FString();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName Instigator = FName();

	FAudioEvent& operator=(const FAudioEvent& rhs) {
		AudioAsset = rhs.AudioAsset;
		AudioType = rhs.AudioType;
		AudioInfo = rhs.AudioInfo;
		Instigator = rhs.Instigator;
		return *this;
	}
};

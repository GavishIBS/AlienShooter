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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	USoundBase* AudioAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EAudioType AudioType = EAudioType::EAT_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString AudioInfo = FString();

	FAudioEvent& operator=(const FAudioEvent& rhs) {
		AudioAsset = rhs.AudioAsset;
		AudioType = rhs.AudioType;
		AudioInfo = rhs.AudioInfo;
		return *this;
	}
};

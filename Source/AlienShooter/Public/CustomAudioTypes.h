// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomAudioTypes.generated.h"

UENUM(BlueprintType)
enum class EAudioType : uint8
{
	EAT_None = 0, EAT_Noise = 1, EAT_Dialogue = 2
};

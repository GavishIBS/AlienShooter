// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "OptionTypes.generated.h"

USTRUCT()
struct FCameraOptions
{
	GENERATED_USTRUCT_BODY()

public:
	FCameraOptions() = default;
	explicit FCameraOptions(const bool bCameraBoomRotation, const bool bCameraBoomOffset)
		: bEnableCameraBoomRotation(bCameraBoomRotation), bEnableCameraBoomOffset(bCameraBoomOffset) {}

	bool bEnableCameraBoomRotation = true;
	bool bEnableCameraBoomOffset = true;  // camera boom can move away from player mesh
};

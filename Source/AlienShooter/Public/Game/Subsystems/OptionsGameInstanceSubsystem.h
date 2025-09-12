// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OptionTypes.h"
#include "OptionsGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UOptionsGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UOptionsGameInstanceSubsystem();

	FCameraOptions CameraSettings;
};

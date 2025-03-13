// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};

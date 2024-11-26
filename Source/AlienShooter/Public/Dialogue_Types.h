// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue_Types.generated.h"

USTRUCT(BlueprintType)
struct FDialogueData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Data)
	FString Dialogue;
	UPROPERTY(EditAnywhere, Category=Data)
	bool bIsReply = false;
};

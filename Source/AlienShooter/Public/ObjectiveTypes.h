// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveTypes.generated.h"

UENUM(BlueprintType)
enum class EObjecttiveType : uint8
{
	NONE = 0, GOTO_POINT = 1, EVENT = 2
};

USTRUCT(BlueprintType)
struct FObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	FString ObjectiveDesc = FString("Objective text");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	bool bOptional = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	EObjecttiveType ObjectiveType = EObjecttiveType::NONE;
};

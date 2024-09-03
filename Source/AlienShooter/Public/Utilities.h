// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Utilities.generated.h"

static float DamageToForceConversion(const float& Damage)
{
	return Damage * Damage * 100000.f;
}

UENUM(BlueprintType)
enum class ELifeStatus : uint8
{
	ELS_None = 0, ELS_Alive = 1, ELS_Dead = 2
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, Category = "Data")
	FName ItemName = FName();

	UPROPERTY(EditAnywhere, Category = "Data")
	TSubclassOf<AActor> ItemClass = nullptr;
};

USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_USTRUCT_BODY();

	FItemData ItemData{};
	bool bEmpty = true;
};

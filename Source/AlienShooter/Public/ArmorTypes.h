// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArmorTypes.generated.h"

USTRUCT(BlueprintType)
struct FArmorProperties
{
	GENERATED_BODY()

	FArmorProperties() = default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	float CurrentArmorHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	float MaxArmorHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (ClampMin = "0", ClampMax = "100"))
	float ArmorDamageAbsorption = 65.f;
};

UENUM(BlueprintType)
enum class EArmorColorCode : uint8
{
	ECC_None = 0, ECC_Green = 1, ECC_Yellow = 2, ECC_Red = 3
};

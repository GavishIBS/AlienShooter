// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DamageTypes.generated.h"

USTRUCT(BlueprintType)
struct FDamageProperties
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, Category = "Properties")
	float BaseDamage = 150.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float MinimumDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamageInnerRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamageOuterRadius = 300.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamageFalloff = 2.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float ForceMag = 500000.f;
};

#pragma once

#include "CoreMinimal.h"
#include "SpecialWeaponTypes.generated.h"

USTRUCT(BlueprintType)
struct FLaserProperties
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects | Laser")
	bool bLaser = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects | Laser | Properties | Limits")
	float Limit_DistanceXY = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects | Laser | Properties | Limits")
	float Limit_DistanceZ = 80.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects | Laser | Properties")
	FLinearColor LaserColor = FLinearColor(1.f, 0.f, 0.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects | Laser | Properties | Limits")
	bool Limit_GlowOnHittableActorsOnly = false;
};

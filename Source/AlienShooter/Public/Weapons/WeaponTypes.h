// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.generated.h"

class AWeaponBase;

UENUM()
enum class EWeaponName : uint8
{
	EWN_None = 0 UMETA(DisplayName="None"),
	EWN_Glock67 = 1 UMETA(DisplayName="Glock 67"),
	EWN_AK47 = 2 UMETA(DisplayName="AK 47")
};

USTRUCT(BlueprintType)
struct FWeaponProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	EWeaponName WeaponName = EWeaponName::EWN_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	bool bIsSkeletalMesh = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FName InHandAttachmentSocketName = FName();
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data)
	TSubclassOf<AWeaponBase> WeaponClass = nullptr;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.generated.h"

class AWeaponBase;
class AProjectile;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FName WeaponMuzzleSocketName = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	float BulletHealth = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	float Damage_RandomizationFactor = 7.5f;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data)
	TSubclassOf<AWeaponBase> WeaponClass = nullptr;
};

USTRUCT(BlueprintType)
struct FProjectileWeaponProperties
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(BlueprintType)
struct FProjectileWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile | Data")
	TSubclassOf<AProjectile> ProjectileClass;
};

USTRUCT(BlueprintType)
struct FHitscanWeaponProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hitscan | Properties", meta=(ClampMin=0, ClampMax=30))
	int32 NumberOfTraces = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hitscan | Properties")
	float Range = 750.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hitscan | Properties")
	float TraceHeightOffset = 10.f;
};

USTRUCT(BlueprintType)
struct FProjectileProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Properties)
	float BulletHealth = 100.f;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Properties)
	float Damage = 20.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Properties)
	float Damage_RandomizationFactor = 7.5f;
};

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

public:
	friend AWeaponBase;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties, meta=(ClampMin=0))
	int AmmoAvailable = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties, meta=(ClampMin=0))
	int MaxAmmoCapacity = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	bool bShouldEnableIK = true;

private:
	bool bIsEmpty = AmmoAvailable <= 0;
	
	int GetTotalAmmoAvailable() const {return AmmoAvailable;}
	void DeductAmmo(const int Value)
	{
		AmmoAvailable -= Value;
		AmmoAvailable = FMath::Clamp(AmmoAvailable, 0, MaxAmmoCapacity);

		if (AmmoAvailable <= 0)
		{
			bIsEmpty = true;
		}
	}
	void AddAmmo(const int Value)
	{
		AmmoAvailable += Value;
		AmmoAvailable = FMath::Clamp(AmmoAvailable, 0, MaxAmmoCapacity);

		if (AmmoAvailable > 0)
		{
			bIsEmpty = false;
		}		
	}
	bool IsEmpty() const {return bIsEmpty;}
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hitscan | Properties", meta=(ClampMin=1, ClampMax=30))
	int32 NumberOfTraces = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hitscan | Properties", meta=(ClampMin=3, ClampMax=30))
	int32 NumberOfHeightDivisions = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hitscan | Properties", meta=(ClampMin=1, ClampMax=20))
	float TraceRadius = 5.f;

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

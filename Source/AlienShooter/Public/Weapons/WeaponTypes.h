// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.generated.h"

class AWeaponBase;
class AProjectile;
class ABulletShell;

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
	TObjectPtr<UTexture2D> WeaponIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FName WeaponLeftHandPlacementSocketName = FName("LeftHandSocket");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FName ShellEjectionSocketName = FName("ShellEjectionSocket");

private:
	bool bShouldEnableRightHandIK = true;
	bool bShouldEnableFabrik = true;

private:
	bool bIsEmpty = AmmoAvailable <= 0;
	
	int GetTotalAmmoAvailable() const {return AmmoAvailable;}
	void SetTotalAmmoAvailable(const int& Value)
	{
		AmmoAvailable = Value > MaxAmmoCapacity ? MaxAmmoCapacity : Value;
	}
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

	UPROPERTY(EditDefaultsOnly, Category=Data)
	TSubclassOf<ABulletShell> BulletShellClass = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponAnimationData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<TObjectPtr<UAnimationAsset>> WeaponFireAnimations;
};

USTRUCT(BlueprintType)
struct FWeaponAnimationData_Pawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<TObjectPtr<UAnimMontage>> FireAnimationMontages_Pawn;   // Character animations
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Motion | Limits")
	float Limit_DistanceZ = 80.f;
};

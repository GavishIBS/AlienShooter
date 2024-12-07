// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SpecialWeaponTypes.generated.h"

USTRUCT(BlueprintType)
struct FLaserProperties    // for all weapons   TODO: move struct from this header to another header
{
	GENERATED_USTRUCT_BODY()

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

UENUM(BlueprintType)
enum class ESpecialEffectType : uint8
{
	None     = 0    UMETA(DisplayName = "None"),
	Freeze   = 1    UMETA(DisplayName = "Freeze"),
	Burn     = 2    UMETA(DisplayName = "Burn"),
	Poison   = 3    UMETA(DisplayName = "Poison"),
};

UENUM()
enum class EDamageDuration : uint8
{
    None        = 0,
    Instant     = 1,
    Duration    = 2,
};

USTRUCT(BlueprintType)
struct FSpecialEffectData   // general struct for a special effect
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    ESpecialEffectType EffectType = ESpecialEffectType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    EDamageDuration DamageDuration = EDamageDuration::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float DamagePerSecond = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float Duration = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float ApplicationChance = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float SlowdownFactor = 1.0f;

    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data | Visuals")
    //UParticleSystem* EffectParticle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float ImpactDamage = 0.0f;
};

USTRUCT()
struct FAcquiredSpecialEffectData
{
    GENERATED_USTRUCT_BODY()

public:
    FAcquiredSpecialEffectData() {}

    ESpecialEffectType EffectType = ESpecialEffectType::None;
    float DamagePerSecond = 0.0f;
    float Duration = 0.0f;
};

/*
USTRUCT(BlueprintType)
struct FFreezeEffectData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    ESpecialEffectType EffectType = ESpecialEffectType::Freeze;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float DamagePerSecond = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float Duration = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data | Visuals")
    UMaterialInterface* OverlayMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float ApplicationChance = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float FreezeSlowdownFactor = 0.f;

    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data | Visuals")
    //UParticleSystem* EffectParticle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float ImpactDamage = 25.f;
};

USTRUCT(BlueprintType)
struct FBurnEffectData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    ESpecialEffectType EffectType = ESpecialEffectType::Burn;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float DamagePerSecond = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float Duration = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data | Visuals")
    UMaterialInterface* OverlayMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float ApplicationChance = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float SlowdownFactor = 1.f;

    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data | Visuals")
    //UParticleSystem* EffectParticle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float ImpactDamage = 50.f;
};

USTRUCT(BlueprintType)
struct FPoisonEffectData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    ESpecialEffectType EffectType = ESpecialEffectType::Poison;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float DamagePerSecond = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float Duration = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data | Visuals")
    UMaterialInterface* OverlayMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float ApplicationChance = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float SlowdownFactor = 0.3f;

    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data | Visuals")
    //UParticleSystem* EffectParticle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Effects | Data")
    float ImpactDamage = 15.f;
};
*/

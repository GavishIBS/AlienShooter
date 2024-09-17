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

namespace AnimationUtils
{
	static bool PlayAnimationMontage(USkeletalMeshComponent* Mesh, TArray<TObjectPtr<UAnimMontage>> Montages, const FName SectionName = FName(), const bool bJumpToSection = false)
	{
		if (!Mesh || Montages.Num() == 0) return false;

		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			const int32 MontageSelection = FMath::RandRange(0, Montages.Num() - 1);
			if (UAnimMontage* Montage = Montages[MontageSelection])
			{
				AnimInstance->Montage_Play(Montage);
				if (bJumpToSection)
				{
					AnimInstance->Montage_JumpToSection(SectionName, Montage);
				}
				return true;
			}
		}
		return false;
	}

	static bool PlayAnimation(USkeletalMeshComponent* Mesh, TArray<TObjectPtr<UAnimationAsset>> Animations, bool bLoop = false)
	{
		if (!Mesh || Animations.Num() == 0) return false;

		const int32 AnimationSelection = FMath::RandRange(0, Animations.Num() - 1);
		if (UAnimationAsset* AnimationAsset = Animations[AnimationSelection])
		{
			Mesh->PlayAnimation(AnimationAsset, bLoop);
			return true;
		}
		return false;
	}
}


UENUM(BlueprintType)
enum class ELifeStatus : uint8
{
	ELS_None = 0, ELS_Alive = 1, ELS_Dead = 2
};

UENUM(BlueprintType)
enum class EPawnActionState : uint8
{
	EPAS_None = 0,
	EPAS_Idle = 1,
	EPAS_Attacking = 2,
	EPAS_UsingItem = 3,
	EPAS_Max = 99
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FName ItemName = FName();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString ItemDescription = FString();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UTexture2D* IconTexture = nullptr;

	UPROPERTY(EditAnywhere, Category = "Data")
	TSubclassOf<AActor> ItemClass = nullptr;
};

USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FItemData ItemData{};

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bEmpty = true;
};

USTRUCT(BlueprintType)
struct FItemProperties
{
	GENERATED_BODY();

public:
	FItemProperties() {};
	FItemProperties(FName InAttachSocketName): AttachmentSocketName(InAttachSocketName) {};

	UPROPERTY(EditAnywhere, Category = "Properties")
	FName AttachmentSocketName = FName("ItemSocket");
};

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
};

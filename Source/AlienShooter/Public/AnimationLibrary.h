// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"

namespace AnimationLibrary
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
// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Interfaces/HitInterface.h"

float IHitInterface::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor,
	AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	return 0.f;
}

void IHitInterface::GetSpecialHit(const FVector& HitLocation, const FSpecialEffectData& SpecialEffectData,
	const float& RandomizationFactor, AActor* InDamageCauser)
{
	return;
}

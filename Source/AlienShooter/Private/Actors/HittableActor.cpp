// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/HittableActor.h"

AHittableActor::AHittableActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHittableActor::BeginPlay()
{
	Super::BeginPlay();
}

float AHittableActor::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser)
{
	return 0.f;
}

void AHittableActor::SetLootData(const FLootData& InLootData)
{
	LootData = InLootData;
}

void AHittableActor::SpawnLoot()
{

}

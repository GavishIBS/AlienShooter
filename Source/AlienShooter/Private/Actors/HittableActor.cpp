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

float AHittableActor::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType)
{
	return 0.f;
}

void AHittableActor::SetLootData(const FLootData& InLootData)
{
	LootData = InLootData;
}

void AHittableActor::SpawnLoot()
{
	if (!LootData.LootItemClass) return;
	
	UWorld* World = GetWorld();
	if (!World) return;

	while (LootData.Quantity > 0)
	{
		const FTransform SpawnTransform = FTransform(GetActorRotation(), GetActorLocation(), FVector(1.f, 1.f, 1.f));
		const FActorSpawnParameters ActorSpawmParams;
		World->SpawnActor<AActor>(LootData.LootItemClass, SpawnTransform, ActorSpawmParams);
		
		LootData.Quantity--;
	}
}

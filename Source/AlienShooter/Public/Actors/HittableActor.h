// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/LootInterface.h"
#include "LootTypes.h"
#include "HittableActor.generated.h"

class URadialFalloff;
class UFieldSystem;

UCLASS()
class ALIENSHOOTER_API AHittableActor : public AActor, public IHitInterface, public ILootInterface
{
	GENERATED_BODY()
	
public:	
	AHittableActor();

	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser) override;

	virtual void SetLootData(const FLootData& InLootData) override;
	virtual void SpawnLoot() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Data | Loot")
	FLootData LootData = FLootData();
};

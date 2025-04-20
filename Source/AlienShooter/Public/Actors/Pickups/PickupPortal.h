// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupPortal.generated.h"

class UStaticMeshComponent;
class APickup;

UCLASS()
class ALIENSHOOTER_API APickupPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupPortal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category=Components)
	TObjectPtr<UStaticMeshComponent> PortalMesh;

	UPROPERTY(VisibleAnywhere, Category=Components)
	TObjectPtr<USceneComponent> ItemSpawnPoint;

	void SpawnItem();

	UFUNCTION()
	void SpawnItemWithDelay(AActor* LastDestroyedPickup);
	
	UPROPERTY(EditAnywhere, Category = "Data")
	TSubclassOf<APickup> PickupItemClass;

	UPROPERTY(EditAnywhere, Category = "Properites")
	float SpawnDelay = 5.f;
};

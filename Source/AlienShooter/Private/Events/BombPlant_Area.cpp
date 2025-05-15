// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Events/BombPlant_Area.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InventoryComponent.h"
#include "Actors/Dynamite.h"

ABombPlant_Area::ABombPlant_Area()
{
	BombPlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombPlatformMesh"));
	BombPlatformMesh->SetupAttachment(GetRootComponent());
	BombPlatformMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	BombPlantPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BombPlantPoint"));
	BombPlantPoint->SetupAttachment(BombPlatformMesh);
}

void ABombPlant_Area::BeginPlay()
{
	Super::BeginPlay();
}

void ABombPlant_Area::BindCallbacks()
{
	Super::BindCallbacks();
	OnEnterArea.AddDynamic(this, &ThisClass::TryPlantBomb);
}

void ABombPlant_Area::TryPlantBomb(AActor* OtherActor)
{
	if (!Bomb_Class) return;

	if (UInventoryComponent* InventoryComponent = OtherActor->FindComponentByClass<UInventoryComponent>())
	{
		if (InventoryComponent->HaveItem(Bomb_Class) > -1)
		{
			ADynamite* SpawnedDynamite = GetWorld()->SpawnActor<ADynamite>(Bomb_Class);
			if (SpawnedDynamite)
			{
				SpawnedDynamite->PlantDynamiteAtLocation(BombPlantPoint->GetComponentLocation(), BombPlantPoint->GetComponentRotation());
				InventoryComponent->RemoveItemFromInventory(Bomb_Class, true);
				Destroy();
			}
		}
	}
}

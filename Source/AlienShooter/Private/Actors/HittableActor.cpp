// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/HittableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AHittableActor::AHittableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	GeometryCollectionComponent->SetupAttachment(GetRootComponent());
	GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GeometryCollectionComponent->SetSimulatePhysics(true);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AHittableActor::BeginPlay()
{
	Super::BeginPlay();
}

float AHittableActor::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser)
{
	if (GeometryCollectionComponent && GetWorld())
	{
		GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		CreateField();
	}
	return 1000000.f;;
}

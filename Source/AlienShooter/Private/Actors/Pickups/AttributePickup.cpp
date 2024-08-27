// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/AttributePickup.h"
#include "Components/AttributeComponent.h"

AAttributePickup::AAttributePickup()
{
}

void AAttributePickup::BeginPlay()
{
	Super::BeginPlay();
}

void AAttributePickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;

	if (UAttributeComponent* AttributeComponent = OtherActor->FindComponentByClass<UAttributeComponent>())
	{
		switch (AttributeSelection)
		{
		case EAttribute::HEALTH:
			AttributeComponent->AddHealth(AdditiveValue);
			break;
		case EAttribute::NONE:
		default:
			break;
		}
	}

	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

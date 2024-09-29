// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/Armor.h"
#include "Components/AttributeComponent.h"

AArmor::AArmor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AArmor::BeginPlay()
{
	Super::BeginPlay();
}

void AArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = Mesh->GetComponentRotation();
	NewRotation.Yaw += 0.5f;
	Mesh->SetWorldRotation(NewRotation);
}

void AArmor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;

	if (UAttributeComponent* AttributeComponent = OtherActor->FindComponentByClass<UAttributeComponent>())
	{
		if (AttributeComponent->CanTakeArmor(ArmorColorCode))
		{
			AttributeComponent->TakeArmor(this);
			Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex,
				bFromSweep, SweepResult);
		}
	}
}

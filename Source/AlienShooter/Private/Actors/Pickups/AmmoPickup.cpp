// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/AmmoPickup.h"
#include "Components/CombatComponent.h"

AAmmoPickup::AAmmoPickup()
{
}

void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;

	if (UCombatComponent* CombatComponent = OtherActor->FindComponentByClass<UCombatComponent>())
	{
		if (CombatComponent->TakeAmmo(FitWeaponClass, AmmoValue))
		{
			Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex,
				bFromSweep, SweepResult);
		}
	}
}

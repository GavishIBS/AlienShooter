// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/ElectricitySwitchboard.h"
#include "Engine/Light.h"
#include "Components/LightComponent.h"
#include "Components/BoxComponent.h"
#include "Actors/ElectronicActor.h"

AElectricitySwitchboard::AElectricitySwitchboard()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(Mesh);
}

void AElectricitySwitchboard::BeginPlay()
{
	Super::BeginPlay();
	BindCallbacks();
}

void AElectricitySwitchboard::BindCallbacks()
{
	if (OverlapBox)
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AElectricitySwitchboard::OnBoxOverlap);
}

void AElectricitySwitchboard::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		OnElectricityStateChanged.Broadcast(true);
		if (TurnOnDelay == 0)
		{
			for (AElectronicActor* Actor : ControlledActors)
				if (Actor && Actor->HaveElectricity() == false)
					Actor->SetHaveElectricity(true);
		}
		else {
			LightsToTurnOn = ControlledActors;
			GetWorldTimerManager().SetTimer(TurnOnLightIteratorDelayTimerHandle, this, &AElectricitySwitchboard::TurnOnLight, TurnOnDelay, true, 0);
		}
		OverlapBox->SetGenerateOverlapEvents(false);
	}
}

void AElectricitySwitchboard::TurnOnLight()
{
	if (LightsToTurnOn.Num() > 0 && LightsToTurnOn[0] != nullptr && LightsToTurnOn[0]->HaveElectricity() == false)
	{
		LightsToTurnOn[0]->SetHaveElectricity(true);
		LightsToTurnOn.RemoveAt(0);
	}
	else {
		GetWorldTimerManager().ClearTimer(TurnOnLightIteratorDelayTimerHandle);
	}
}

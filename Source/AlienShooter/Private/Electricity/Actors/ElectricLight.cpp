// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/ElectricLight.h"
#include "Components/LightComponent.h"

AElectricLight::AElectricLight()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AElectricLight::BeginPlay()
{
	Super::BeginPlay();
	if (Light) Light->SetVisibility(false);
}

void AElectricLight::OnElectricityTurnedOn()
{
	if (Light)
		Light->SetVisibility(true);
}

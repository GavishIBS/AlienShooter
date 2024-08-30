// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/ElectricSpotLight.h"
#include "Components/SpotLightComponent.h"

AElectricSpotLight::AElectricSpotLight()
{
	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(GetRootComponent());
}

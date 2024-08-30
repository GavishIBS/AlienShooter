// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/ElectricPointLight.h"
#include "Components/PointLightComponent.h"

AElectricPointLight::AElectricPointLight()
{
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(GetRootComponent());
}

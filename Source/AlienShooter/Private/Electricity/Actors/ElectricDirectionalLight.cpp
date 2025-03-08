// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/ElectricDirectionalLight.h"
#include "Components/DirectionalLightComponent.h"

AElectricDirectionalLight::AElectricDirectionalLight()
{
	Light = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Light"));
	Light->SetupAttachment(GetRootComponent());
}

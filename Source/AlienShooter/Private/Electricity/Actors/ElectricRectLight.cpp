// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/ElectricRectLight.h"
#include "Components/RectLightComponent.h"

AElectricRectLight::AElectricRectLight()
{
	Light = CreateDefaultSubobject<URectLightComponent>(TEXT("Light"));
	Light->SetupAttachment(GetRootComponent());
}

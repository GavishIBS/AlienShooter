// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/AlarmDirectionalLight_Electric.h"
#include "Components/LightComponent.h"

AAlarmDirectionalLight_Electric::AAlarmDirectionalLight_Electric()
{
	Light->SetVisibility(false);
}

void AAlarmDirectionalLight_Electric::SetLightVisibility(const bool InValue)
{
	Light->SetVisibility(InValue);
}

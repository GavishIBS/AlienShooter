// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/Electric_Siren_SpotLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/LightComponent.h"

AElectric_Siren_SpotLight::AElectric_Siren_SpotLight()
{
	SirenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SirenMesh"));
	SetRootComponent(SirenMesh);
	
	Light->SetupAttachment(GetRootComponent());
	Light->SetVisibility(false);
}

void AElectric_Siren_SpotLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Light && Light->IsVisible()) {
		Light->AddLocalRotation(SirenRotationSpeed);
	}
}

void AElectric_Siren_SpotLight::TurnOnLightWithMode(const EAlarmingModeName& ModeName)
{
	SetLightProperties(ModeName);
	TurnOnLight();
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/ElectricLight.h"
#include "Components/LightComponent.h"

#include "Kismet/KismetMathLibrary.h"

AElectricLight::AElectricLight()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AElectricLight::BeginPlay()
{
	Super::BeginPlay();
	MaxIntensity = Light->Intensity;
	if (Light) Light->SetVisibility(false);
}

void AElectricLight::OnElectricityTurnedOn()
{
	if (Light && bTurnOnAuto)
	{
		if (bInterpolateIntensity) {
			Light->SetIntensity(0.f);
			bInterpingIntensity = true;
		}
		Light->SetVisibility(true);
	}

	if (Light && Light->IsVisible() && bFlicker) {
		if (FlickerHoldTime > FlickerRate) {
			FlickerHoldTime /= 2;
		}
		FTimerHandle FlickerTimerHandle;
		GetWorldTimerManager().SetTimer(FlickerTimerHandle, this, &AElectricLight::FlickerLight, FlickerRate, true, FlickerRate);
	}
}

void AElectricLight::FlickerLight()
{
	Light->SetVisibility(false);
	
	const float L_FlickerHoldTime = FMath::FRandRange(FlickerHoldTime - (FlickerHoldTime / 3), FlickerHoldTime + (FlickerHoldTime / 3));
	FTimerHandle FlickerHoldTimerHandle;
	GetWorldTimerManager().SetTimer(FlickerHoldTimerHandle, FTimerDelegate::CreateLambda([&] {
		Light->SetVisibility(true);
		}), L_FlickerHoldTime, false, L_FlickerHoldTime);
}

void AElectricLight::Tick(float DeltaTime)
{
	if (bInterpingIntensity)
	{
		if (Light->Intensity >= MaxIntensity) bInterpingIntensity = false;
		Light->SetIntensity(UKismetMathLibrary::FInterpTo(Light->Intensity, MaxIntensity, DeltaTime, IntensityInterpSpeed));
	}
}

void AElectricLight::TurnOnLight()
{
	if (HaveElectricity()) {
		if (Light) {
			if (bInterpolateIntensity) {
				Light->SetIntensity(0.f);
				bInterpingIntensity = true;
			}
			Light->SetVisibility(true);
		}
	}
}

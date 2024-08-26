// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Lighting/LightSwitchboard.h"
#include "Engine/Light.h"
#include "Components/LightComponent.h"
#include "Components/BoxComponent.h"

ALightSwitchboard::ALightSwitchboard()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(Mesh);
}

void ALightSwitchboard::BeginPlay()
{
	Super::BeginPlay();
	BindCallbacks();
}

void ALightSwitchboard::BindCallbacks()
{
	if (OverlapBox)
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ALightSwitchboard::OnBoxOverlap);
}

void ALightSwitchboard::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ToggleVisibility(true);
		OverlapBox->SetGenerateOverlapEvents(false);
	}
}

void ALightSwitchboard::ToggleVisibility(bool bVisibility)
{
	for (ALight* L_Ins : ControlledLightInstances)
		L_Ins->GetLightComponent()->SetVisibility(bVisibility);
}

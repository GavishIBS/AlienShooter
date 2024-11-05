// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "GameCharacter.h"
#include "Components/CapsuleComponent.h"

AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGameCharacter::StartStopwatch()
{
	GetWorldTimerManager().SetTimer(StopwatchTimerHandle, this, &AGameCharacter::StopwatchTick, 1.f, true, 0.f);
}

void AGameCharacter::StopStopwatch()
{
	GetWorldTimerManager().ClearTimer(StopwatchTimerHandle);
}

void AGameCharacter::StopwatchTick()
{
}

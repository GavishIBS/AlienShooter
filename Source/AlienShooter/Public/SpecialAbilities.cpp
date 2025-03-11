// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#include "SpecialAbilities.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

FTimerHandle Abilities::DashExecutionEndTimerHandle;
ACharacter* Abilities::Char = nullptr;

Abilities::Abilities()
{
}

Abilities::~Abilities()
{
}

bool Abilities::Dash(ACharacter* InCharacter, float ExecutionDuration) {
	if (!InCharacter) return false;

	/*
	FRotator ForwardDirectionRot = InCharacter->GetMesh()->GetComponentRotation();
	ForwardDirectionRot.Yaw += 90.f;
	FVector LaunchDirection = UKismetMathLibrary::GetForwardVector(ForwardDirectionRot);
	*/

	InCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Abilities::Char = InCharacter;

	FVector LaunchDirection = InCharacter->GetVelocity().GetSafeNormal();
	InCharacter->LaunchCharacter(LaunchDirection * 3250.f, false, false);
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString("I am dashing"));


	InCharacter->GetWorld()->GetTimerManager().SetTimer(
		DashExecutionEndTimerHandle,
		[]() { Abilities::OnDashExecutionEnd(); },
		ExecutionDuration,
		false
	);

	return true;
}

void Abilities::OnDashExecutionEnd()
{
	if (Abilities::Char)
		Abilities::Char->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Abilities::Char = nullptr;
}

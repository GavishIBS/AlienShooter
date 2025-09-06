// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#include "SpecialAbilities.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

FTimerHandle Abilities::AbilityExecutionEndTimerHandle;
ACharacter* Abilities::Char = nullptr;
float Abilities::CacheFloat = 0.f;

Abilities::Abilities()
{
}

Abilities::~Abilities()
{
}

void Abilities::EndAllAbilities()
{
	if (Char) {
		Char->GetWorld()->GetTimerManager().ClearTimer(AbilityExecutionEndTimerHandle);
	}

	OnDashExecutionEnd();
	OnSpeedBoostExecutionEnd();
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
		AbilityExecutionEndTimerHandle,
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

bool Abilities::SpeedBoost(ACharacter* InCharacter, float ExecutionDuration)
{
	Abilities::Char = InCharacter;

	if (InCharacter->GetCharacterMovement())
	{
		Abilities::CacheFloat = InCharacter->GetCharacterMovement()->MaxWalkSpeed;
		InCharacter->GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	}

	InCharacter->GetWorld()->GetTimerManager().SetTimer(
		AbilityExecutionEndTimerHandle,
		[]() { Abilities::OnSpeedBoostExecutionEnd(); },
		ExecutionDuration,
		false
	);

	return true;
}

void Abilities::OnSpeedBoostExecutionEnd()
{
	if (Abilities::Char) {
		Char->GetCharacterMovement()->MaxWalkSpeed = Abilities::CacheFloat;
	}
	Abilities::Char = nullptr;

}

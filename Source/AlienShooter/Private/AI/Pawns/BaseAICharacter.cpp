// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/Pawns/BaseAICharacter.h"
#include "AI/Controller/AIControllerBase.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!IsInAttackRange())
		ChaseTarget(AIController);
}

void ABaseAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AIController = Cast<AAIControllerBase>(NewController);
	if (!AIController) Destroy();
	InitCallbacks();
	if (const bool bUpdated = UpdateChaseTarget(AIController); !bUpdated) Destroy();
}

void ABaseAICharacter::InitCallbacks()
{
	OnTakeAnyDamage.AddDynamic(this, &ABaseAICharacter::OnTakeAnyDamageCallback);
}

float ABaseAICharacter::GetHit(const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser)
{
	const float Damage_Calc = Damage +
		UKismetMathLibrary::RandomFloatInRange(-RandomizationFactor, RandomizationFactor);
	const FDamageEvent DamageEvent;
	TakeDamage(Damage_Calc, DamageEvent, nullptr, InDamageCauser);
	return Damage_Calc;
}

void ABaseAICharacter::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	float NewHealth = AIController->GetAIProperties().Health - Damage;
	NewHealth = FMath::Clamp(NewHealth, 0.f, AIController->GetAIProperties().MaxHealth);
	AIController->SetHealth(NewHealth);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("NewHealth: %f"), NewHealth));

	if (NewHealth <= 0.f)
	{
		// Death
		// TODO: Implementation
	}
}

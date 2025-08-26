// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/Pawns/BaseAIPawn.h"
#include "AI/Controller/AIControllerBase.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"

ABaseAIPawn::ABaseAIPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("TeamFungus"));
}

void ABaseAIPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAIPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!IsInAttackRange())
		ChaseTarget(AIController);
}

void ABaseAIPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AIController = Cast<AAIControllerBase>(NewController);
	if (!AIController) Destroy();
	InitCallbacks();
	if (const bool bUpdated = UpdateChaseTarget(AIController); !bUpdated) Destroy();
}

void ABaseAIPawn::InitCallbacks()
{
	OnTakeAnyDamage.AddDynamic(this, &ABaseAIPawn::OnTakeAnyDamageCallback);
}

float ABaseAIPawn::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType)
{
	const float Damage_Calc = Damage +
		UKismetMathLibrary::RandomFloatInRange(-RandomizationFactor, RandomizationFactor);
	const FDamageEvent DamageEvent;
	TakeDamage(Damage_Calc, DamageEvent, nullptr, InDamageCauser);
	return Damage_Calc;
}

void ABaseAIPawn::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
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

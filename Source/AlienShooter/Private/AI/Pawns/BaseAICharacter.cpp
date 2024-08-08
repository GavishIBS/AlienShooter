// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/Pawns/BaseAICharacter.h"
#include "AI/Controller/AIControllerBase.h"
#include "Components/CollisionTraceComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("TeamFungus"));
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	CollisionTraceComponent = CreateDefaultSubobject<UCollisionTraceComponent>(TEXT("CollisionTraceComponent"));
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsInAttackRange())
		Attack();
	else ChaseTarget(AIController);
}

void ABaseAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AIController = Cast<AAIControllerBase>(NewController);
	if (!AIController) Destroy();
	InitCallbacks();
	if (const bool bUpdated = UpdateChaseTarget(AIController); !bUpdated) Destroy();
}

void ABaseAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CollisionTraceComponent)
		CollisionTraceComponent->InitComponent(GetMesh());
}

void ABaseAICharacter::InitCallbacks()
{
	OnTakeAnyDamage.AddDynamic(this, &ABaseAICharacter::OnTakeAnyDamageCallback);
}

float ABaseAICharacter::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser)
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

	if (NewHealth <= 0.f)
	{
		// Death
		// TODO: Implementation
	}
}

bool ABaseAICharacter::CanAttack() const
{
	if (!AIController) return false;
	if (AIController->GetAIState() >= EAS_Attacking) return false;
	return true;
}

void ABaseAICharacter::Attack() const
{
	if (!CanAttack()) return;
	if (AIAnimations.AttackAnimMontages.Num() == 0) return;
	
	const int MontageSelectionIndex = FMath::RandRange(0, AIAnimations.AttackAnimMontages.Num() - 1);

	if (!AIAnimations.AttackAnimMontages[MontageSelectionIndex]) return;

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(AIAnimations.AttackAnimMontages[MontageSelectionIndex]);
		AIController->SetAIState(EAS_Attacking);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Playing attack montage"));
	}
}

void ABaseAICharacter::StartAttackTrace()
{
	GetWorldTimerManager().SetTimer(AttackTraceTimerHandle, this, &ABaseAICharacter::AttackTrace,
		AttackTraceRate, true);
}

void ABaseAICharacter::EndAttackTrace()
{
	GetWorldTimerManager().ClearTimer(AttackTraceTimerHandle);
}

bool ABaseAICharacter::IsInAttackRange()
{
	if (!AIController) return false;

	if (FMath::Abs(AIController->TargetCharacter->GetActorLocation().Size() - GetActorLocation().Size())
		< AIController->GetAIProperties().AttackRange)
	{
		return true;
	}
	return false;
}

void ABaseAICharacter::OnEndAttack()
{
	if (AIController)
		AIController->SetAIState(EAS_Idle);
}

void ABaseAICharacter::AttackTrace()
{
	IAIInterface::AttackTrace();
	if (!CollisionTraceComponent) return;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	const FHitResult HitResult = CollisionTraceComponent->DoTraceSingle(ActorsToIgnore, 0); // TODO: Attack index (2 attacks for each enemy)

	if (!HitResult.bBlockingHit || HitResult.GetActor()->ActorHasTag("TeamFungus")) return;
	
	if (IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor()))
	{
		HitInterface->GetHit(HitResult.ImpactPoint, AIController->GetAIProperties().AttackDamage,
			AIController->GetAIProperties().AttackDamageRandomizationFactor, this);
		EndAttackTrace();
	}
}

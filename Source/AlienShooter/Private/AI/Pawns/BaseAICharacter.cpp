// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/Pawns/BaseAICharacter.h"
#include "AI/Controller/AIControllerBase.h"
#include "Components/CollisionTraceComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("TeamFungus"));
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

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

void ABaseAICharacter::Destroyed()
{
	Super::Destroyed();
	
	UNiagaraSystem* NiagaraSystem = nullptr;
	if (!DeathParticleSystem.Get())
		DeathParticleSystem.LoadSynchronous();
	NiagaraSystem = DeathParticleSystem.Get();
	if (NiagaraSystem)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(),
			GetActorRotation(), GetActorScale3D(), true, true);

	if (DeathDecalMaterialInstance)
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult OutHit;
		const FVector Start = GetActorLocation();
		const FVector End = GetActorLocation() - FVector(0.f, 0.f, 1000.f);
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1,
			false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true);
		if (OutHit.bBlockingHit)
		{
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
				DeathDecalMaterialInstance, DeathDecalSize, OutHit.ImpactPoint,
				FRotator(-90.f, 0.f, 0.f), DeathDecalLifeSpawn);
		}
	}
}

void ABaseAICharacter::InitCallbacks()
{
	OnTakeAnyDamage.AddDynamic(this, &ABaseAICharacter::OnTakeAnyDamageCallback);
	OnDeath.AddDynamic(this, &ABaseAICharacter::OnDeathCallback);
}

float ABaseAICharacter::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	if (LifeStatus != ELifeStatus::ELS_Alive) return 0.f;

	const float Damage_Calc = Damage +
		UKismetMathLibrary::RandomFloatInRange(-RandomizationFactor, RandomizationFactor);
	const FDamageEvent DamageEvent;
	TakeDamage(Damage_Calc, DamageEvent, nullptr, InDamageCauser);
	return Damage_Calc;
}

void ABaseAICharacter::GetSpecialHit(const FVector& HitLocation, const FSpecialEffectData& SpecialEffectData,
	const float& RandomizationFactor, AActor* InDamageCauser)
{
	if (LifeStatus != ELifeStatus::ELS_Alive) return;

	if (SpecialEffectData.ApplicationChance < 1.f)
	{
		if (SpecialEffectData.ApplicationChance < FMath::RandRange(0.f, 1.f))
			return;
	}

	const float Damage_Calc = SpecialEffectData.ImpactDamage +
		UKismetMathLibrary::RandomFloatInRange(-RandomizationFactor, RandomizationFactor);
	const FDamageEvent DamageEvent;
	TakeDamage(Damage_Calc, DamageEvent, nullptr, InDamageCauser);

	int NumMaterials = GetMesh()->GetNumMaterials();
	for (int i = 0; i < NumMaterials; i++)
	{
		UMaterialInterface* Material = GetMesh()->GetMaterial(i);
		
		UMaterialInstanceDynamic* DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(Material);
		if (!DynamicMaterialInstance && Material)
		{

			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
			GetMesh()->SetMaterial(i, DynamicMaterialInstance);
		}

		if (DynamicMaterialInstance)
		{
			DynamicMaterialInstance->SetScalarParameterValue(TEXT("SpecialEffect"), 1);

			switch (SpecialEffectData.EffectType)
			{
				case ESpecialEffectType::Poison:
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Poisoned"), 1);
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Frozen"), 0);
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Burned"), 0);
					break;
				case ESpecialEffectType::Freeze:
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Poisoned"), 0);
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Frozen"), 1);
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Burned"), 0);
					break;
				case ESpecialEffectType::Burn:
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Poisoned"), 0);
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Frozen"), 0);
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Burned"), 1);
					break;
				default:
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Poisoned"), 0);
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Frozen"), 0);
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("Burned"), 0);
					break;
			}
		}
	}

	CustomTimeDilation *= SpecialEffectData.SlowdownFactor;

	AcquiredSpecialEffectData.EffectType = SpecialEffectData.EffectType;
	AcquiredSpecialEffectData.DamagePerSecond = SpecialEffectData.DamagePerSecond;
	AcquiredSpecialEffectData.Duration = SpecialEffectData.Duration;

	if (SpecialEffectData.DamageDuration > EDamageDuration::Instant)
	{
		StartStopwatch();
	}
}

void ABaseAICharacter::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (LifeStatus != ELifeStatus::ELS_Alive) return;

	float NewHealth = AIController->GetAIProperties().Health - Damage;
	NewHealth = FMath::Clamp(NewHealth, 0.f, AIController->GetAIProperties().MaxHealth);
	AIController->SetHealth(NewHealth);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("NewHealth: %f"), NewHealth));

	if (NewHealth <= 0.f)
	{
		OnDeath.Broadcast(DamageCauser);
	}
}

void ABaseAICharacter::OnDeathCallback(AActor* Killer)
{
	if (LifeStatus == ELifeStatus::ELS_Dead) return;
	if (AIAnimations.DeathAnimMontage) Destroy();
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		AnimInstance->Montage_Play(AIAnimations.DeathAnimMontage);
	LifeStatus = ELifeStatus::ELS_Dead;
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

void ABaseAICharacter::ShouldDestroy()
{
	Destroy();
}

void ABaseAICharacter::StopwatchTick()
{
	if (LifeStatus == ELifeStatus::ELS_Dead) ShouldDestroy();

	if (AcquiredSpecialEffectData.Duration <= 0.f)
	{
		// TODO: character material
		AcquiredSpecialEffectData = FAcquiredSpecialEffectData();
		CustomTimeDilation = 1.f;

		int NumMaterials = GetMesh()->GetNumMaterials();
		for (int i = 0; i < NumMaterials; i++)
		{
			UMaterialInterface* Material = GetMesh()->GetMaterial(i);
			UMaterialInstanceDynamic* DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(Material);
			if (!DynamicMaterialInstance && Material)
			{

				DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
				//DynamicMaterialInstance->SetScalarParameterValue(TEXT("SpecialEffect"), 0);
				GetMesh()->SetMaterial(i, DynamicMaterialInstance);
			}

			if (DynamicMaterialInstance)
			{
				DynamicMaterialInstance->SetScalarParameterValue(TEXT("SpecialEffect"), 0);
			}
		}

		StopStopwatch();
		return;
	}

	const float Damage_Calc = AcquiredSpecialEffectData.DamagePerSecond;
	const FDamageEvent DamageEvent;
	TakeDamage(Damage_Calc, DamageEvent, nullptr, nullptr);

	AcquiredSpecialEffectData.Duration -= 1.f;
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

	if (FVector::Dist(AIController->TargetCharacter->GetActorLocation(), GetActorLocation())
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
			AIController->GetAIProperties().AttackDamageRandomizationFactor, this, UDamageType::StaticClass());
		EndAttackTrace();
	}
}

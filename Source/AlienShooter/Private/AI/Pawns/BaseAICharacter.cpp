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
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add(FName("TeamFungus"));
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	CollisionTraceComponent = CreateDefaultSubobject<UCollisionTraceComponent>(TEXT("CollisionTraceComponent"));
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetCharacterMovement() != nullptr) {
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed + FMath::FRandRange(-SpeedRandomizationRange, SpeedRandomizationRange);
	}

	TWeakObjectPtr<ABaseAICharacter> WeakThis(this);

	FTimerHandle CheckCanAttackTimerHandle;
	GetWorldTimerManager().SetTimer(CheckCanAttackTimerHandle, FTimerDelegate::CreateLambda([WeakThis]{
		
		if (!WeakThis.IsValid()) return;  // Actor destroyed? Do nothing

		if (WeakThis->AIController->TargetCharacter) {
			if (WeakThis->IsInAttackRange())
				WeakThis->Attack();
			else {

				WeakThis->ChaseTarget(WeakThis->AIController);
			}
		}
	}), 0.16f, true, 0.16f);

	// Setup target forget timer
	if (bCanForgetTarget == true && bOnlyForgetWhenNoPathFound == false)
	{
		GetWorldTimerManager().SetTimer(ForgetTargetTimerHandle, this, &ABaseAICharacter::ForgetTarget, ForgetTargetRate, true, ForgetTargetRate / 2);
	}
	else if (bCanForgetTarget == true)
	{
		GetWorldTimerManager().SetTimer(ForgetTargetTimerHandle, FTimerDelegate::CreateLambda([WeakThis] {
			
			if (!WeakThis.IsValid()) return;  // Actor destroyed? Do nothing

			if (WeakThis->AIController) {
				//bool CanReach = WeakThis->AIController->CanReachActor(WeakThis->AIController->TargetCharacter);
				bool CanReach = WeakThis->AIController->CanReachActor(UGameplayStatics::GetPlayerCharacter(WeakThis->GetWorld(), 0));
				if (!CanReach) {
					WeakThis->ForgetTarget();
				}
				else if (WeakThis->AIController->TargetCharacter == nullptr && WeakThis->bOnlyForgetWhenNoPathFound == true) {
					WeakThis->ReaquireTarget();
				}
			}
		}), PathCheckRate, true, PathCheckRate);
	}
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

	UMaterialInstance* DeathDecalMI_Selection = 0;
	if (DeathDecalMaterialInstances.Num() > 0) {
		int32 DeathDecalIndex = UKismetMathLibrary::RandomIntegerInRange(0, DeathDecalMaterialInstances.Num() - 1);
		DeathDecalMI_Selection = DeathDecalMaterialInstances[DeathDecalIndex];
	}
	if (DeathDecalMI_Selection)
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult OutHit;
		const FVector Start = GetActorLocation();
		const FVector End = GetActorLocation() - FVector(0.f, 0.f, 1000.f);
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1,
			false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);
		if (OutHit.bBlockingHit)
		{
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
				DeathDecalMI_Selection, DeathDecalSize, OutHit.ImpactPoint,
				FRotator(-90.f, 0.f, 0.f), DeathDecalLifeSpawn);
		}
	}
}

void ABaseAICharacter::InitCallbacks()
{
	OnTakeAnyDamage.AddDynamic(this, &ABaseAICharacter::OnTakeAnyDamageCallback);
	OnDeath.AddDynamic(this, &ABaseAICharacter::OnDeathCallback);
}

float ABaseAICharacter::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor,
	AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType)
{
	if (LifeStatus != ELifeStatus::ELS_Alive) {
		return 0.f;
	}

	if (OutGetHitReturnType && AIController->GetAIProperties().Health <= Damage) {
		*OutGetHitReturnType = EGetHitReturnType::MonsterKill;
	}

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

bool ABaseAICharacter::CanForgetTarget()
{
	if (AIController == nullptr || AIController->TargetCharacter == nullptr) return false;

	if (FVector::Dist(AIController->TargetCharacter->GetActorLocation(), GetActorLocation())
		> TargetForgetMinRange)
	{
		return true;
	}
	return false;
}

void ABaseAICharacter::ForgetTarget()
{
	if (CanForgetTarget() == false) return;

	AIController->SetTargetCharacter(nullptr);

	// No target AI movement
	if (bStopOnTargetForget) {
		// Stop AI
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor((AActor*)this);
		MoveRequest.SetAcceptanceRadius(AIController->GetAIProperties().AcceptanceRadius);
		AIController->MoveTo(MoveRequest);
	}
	else {
		// Random AI movement
		TWeakObjectPtr<ABaseAICharacter> WeakThis(this);
		GetWorldTimerManager().SetTimer(NoTargetRandomMoveTickTimerHandle, FTimerDelegate::CreateLambda([WeakThis]() {
			
			if (!WeakThis.IsValid()) return;  // Actor destroyed? Do nothing
			
			FNavLocation ResultLocation;
			UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(WeakThis->GetWorld());
			if (NavSys && NavSys->GetRandomPointInNavigableRadius(WeakThis->GetActorLocation(), WeakThis->NoTargetMoveRandomPointArea, ResultLocation))
			{
				FAIMoveRequest MoveRequest;
				MoveRequest.SetAcceptanceRadius(WeakThis->AIController->GetAIProperties().AcceptanceRadius);
				MoveRequest.SetGoalLocation(ResultLocation);
				WeakThis->AIController->MoveTo(MoveRequest);
			}
		}), NoTargetRandomMovementResetRate, true, NoTargetRandomMovementResetRate);
	}
	

	if (bOnlyForgetWhenNoPathFound == false) {
		// Setup timer to reaquire target
		FTimerHandle ReaquireTargetTimerHandle;
		GetWorldTimerManager().SetTimer(ReaquireTargetTimerHandle, this, &ABaseAICharacter::ReaquireTarget, ForgetTargetTime, false, ForgetTargetTime);
	}
}

void ABaseAICharacter::ReaquireTarget()
{
	GetWorldTimerManager().ClearTimer(NoTargetRandomMoveTickTimerHandle);  // Stop no target random movements
	if (const bool bUpdated = UpdateChaseTarget(AIController); !bUpdated) Destroy();  // Try get target otherwise destroy
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

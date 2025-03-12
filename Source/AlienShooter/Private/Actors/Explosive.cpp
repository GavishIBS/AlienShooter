// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.



#include "Actors/Explosive.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Damage/ExplosionDamage.h"
#include "Interfaces/PhysicsActorInterface.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/ExplodeInterface.h"
#include "Kismet/KismetSystemLibrary.h"


#include "Characters/BaseCharacter.h"

AExplosive::AExplosive()
{
	PrimaryActorTick.bCanEverTick = false;

	ExplosiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosiveMesh"));
	ExplosiveMesh->SetupAttachment(GetRootComponent());

	ExplosionOriginPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionOriginPoint"));
	ExplosionOriginPoint->SetupAttachment(ExplosiveMesh);
}

void AExplosive::BeginPlay()
{
	Super::BeginPlay();
	InitCallbacks();
}

void AExplosive::InitCallbacks()
{
	//OnTakeAnyDamage.AddDynamic(this, &AExplosive::OnTakeDamageCallback);
}

/*
void AExplosive::OnTakeDamageCallback(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageCauser == this) return;
	GetHit(GetActorLocation(), Damage, 0.f, DamageCauser, DamageType->GetClass());
}
*/

float AExplosive::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	/*
	// Spawn effects
	UNiagaraSystem* NiagaraSystem = nullptr;
	if (!ExplosionParticleSystem.Get())
		ExplosionParticleSystem.LoadSynchronous();
	NiagaraSystem = ExplosionParticleSystem.Get();
	if (NiagaraSystem)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(),
			GetActorRotation(), GetActorScale3D(), true, true);

	OnTakeAnyDamage.RemoveDynamic(this, &AExplosive::OnTakeDamageCallback);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(InDamageCauser);
	//UGameplayStatics::ApplyRadialDamageWithFalloff(this, DamageProperties.BaseDamage,
		//DamageProperties.MinimumDamage, ExplosionOriginPoint->GetComponentLocation(),
		//DamageProperties.DamageInnerRadius, DamageProperties.DamageOuterRadius,
		//DamageProperties.DamageFalloff, UExplosionDamage::StaticClass(), ActorsToIgnore, InDamageCauser);
	UGameplayStatics::ApplyRadialDamage(this, DamageProperties.BaseDamage,
		ExplosionOriginPoint->GetComponentLocation(), DamageProperties.DamageInnerRadius,
		UExplosionDamage::StaticClass(), ActorsToIgnore);
	
	SpawnLoot();

	// Destroy
	Destroy();
	*/

	// Camera shake // TEMPORARY -> Use a better approach to trigger camera shake, don't access character class directly here
	if (InDamageCauser) {
		if (FVector::Dist(GetActorLocation(), InDamageCauser->GetActorLocation()) < CameraShakeRange) {
			if (ABaseCharacter* BaseChar = Cast<ABaseCharacter>(InDamageCauser)) {
				BaseChar->DoCameraShake(FVector(7, 7, 2.5), 1, false, FVector());
			}
		}
	}

	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosive::ExplodeWrapper, ExplosionDelay, false, ExplosionDelay);
	return DamageProperties.BaseDamage;
}

void AExplosive::Explode(AActor* ThisObject, UParticleSystem* InExplosionParticleSystem, const FDamageProperties& InDamageProperties,
	const FVector& Loc, const FRotator& Rot, const TArray<AActor*> InActorsToIgnore)
{
	IExplodeInterface::Explode(ThisObject, InExplosionParticleSystem, InDamageProperties, Loc, Rot, InActorsToIgnore);
}

void AExplosive::ExplodeWrapper()
{
	/*
	// Spawn effects
	UNiagaraSystem* NiagaraSystem = nullptr;
	if (!ExplosionParticleSystem.Get())
		ExplosionParticleSystem.LoadSynchronous();
	NiagaraSystem = ExplosionParticleSystem.Get();
	if (NiagaraSystem)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(),
			GetActorRotation(), GetActorScale3D(), true, true);

	//OnTakeAnyDamage.RemoveDynamic(this, &AExplosive::OnTakeDamageCallback);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	//ActorsToIgnore.Add(InDamageCauser);
	//UGameplayStatics::ApplyRadialDamageWithFalloff(this, DamageProperties.BaseDamage,
		//DamageProperties.MinimumDamage, ExplosionOriginPoint->GetComponentLocation(),
		//DamageProperties.DamageInnerRadius, DamageProperties.DamageOuterRadius,
		//DamageProperties.DamageFalloff, UExplosionDamage::StaticClass(), ActorsToIgnore, InDamageCauser);
	UGameplayStatics::ApplyRadialDamage(this, DamageProperties.BaseDamage,
		ExplosionOriginPoint->GetComponentLocation(), DamageProperties.DamageInnerRadius,
		UExplosionDamage::StaticClass(), ActorsToIgnore);

	// TODO: Instead of applying damage and calling GetHit separately combine both

	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::SphereTraceMulti(this, ExplosionOriginPoint->GetComponentLocation(), ExplosionOriginPoint->GetComponentLocation(),
		ForceAffectingSphereRadius, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
		OutHits, true);

	TArray<AActor*> OutOverlappingActors;
	for (auto Hit : OutHits) {
		if (Hit.bBlockingHit) {
			OutOverlappingActors.Add(Hit.GetActor());
		}
	}

	for (int i = 0; i < OutOverlappingActors.Num(); i++) {
		if (OutOverlappingActors[i] == this) continue;
		if (OutOverlappingActors[i] == nullptr) continue;
		if (IHitInterface* HI = Cast<IHitInterface>(OutOverlappingActors[i])) {
			HI->GetHit(OutOverlappingActors[i]->GetActorLocation(), DamageProperties.BaseDamage, 1.f, this, UExplosionDamage::StaticClass());
		}
		if (IPhysicsActorInterface* PAI = Cast<IPhysicsActorInterface>(OutOverlappingActors[i])) {
			if (PAI && PAI->GetPhysicsComponent()) {
				FVector Direction = OutOverlappingActors[i]->GetActorLocation() - ExplosionOriginPoint->GetComponentLocation();
				Direction = Direction / FVector(FMath::Abs(Direction.X), FMath::Abs(Direction.Y), FMath::Abs(Direction.Z));
				PAI->GetPhysicsComponent()->AddImpulse(Direction * ForceMagnitude);
			}
		}
	}
	*/
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	Explode(this, ExplosionParticleSystem_C, DamageProperties, ExplosionOriginPoint->GetComponentLocation(),
		ExplosionOriginPoint->GetComponentRotation(), ActorsToIgnore);
	OnExplosion();
}

void AExplosive::OnExplosion()
{
	SpawnLoot();
	Destroy();
}

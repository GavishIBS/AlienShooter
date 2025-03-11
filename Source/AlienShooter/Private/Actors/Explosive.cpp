// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.



#include "Actors/Explosive.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Damage/ExplosionDamage.h"
#include "Interfaces/PhysicsActorInterface.h"
//#include "DrawDebugHelpers.h"

#include "Characters/BaseCharacter.h"

AExplosive::AExplosive()
{
	PrimaryActorTick.bCanEverTick = false;

	ExplosiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosiveMesh"));
	ExplosiveMesh->SetupAttachment(GetRootComponent());

	ExplosionOriginPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionOriginPoint"));
	ExplosionOriginPoint->SetupAttachment(ExplosiveMesh);

	ForceAffectingAreaCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ForceAffectingAreaCollision"));
	ForceAffectingAreaCollision->SetupAttachment(ExplosionOriginPoint);
}

void AExplosive::BeginPlay()
{
	Super::BeginPlay();
	ForceAffectingAreaCollision->SetSphereRadius(ForceAffectingSphereRadius);
	//DrawDebugSphere(GetWorld(), ForceAffectingAreaCollision->GetComponentLocation(), ForceAffectingSphereRadius, 24, FColor::Green, true);
	InitCallbacks();
}

void AExplosive::InitCallbacks()
{
	OnTakeAnyDamage.AddDynamic(this, &AExplosive::OnTakeDamageCallback);
}

void AExplosive::OnTakeDamageCallback(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageCauser == this) return;
	GetHit(GetActorLocation(), Damage, 0.f, DamageCauser, DamageType->GetClass());
}

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

	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosive::Explode, ExplosionDelay, false, ExplosionDelay);
	return DamageProperties.BaseDamage;
}

void AExplosive::Explode()
{
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
	//ActorsToIgnore.Add(InDamageCauser);
	//UGameplayStatics::ApplyRadialDamageWithFalloff(this, DamageProperties.BaseDamage,
		//DamageProperties.MinimumDamage, ExplosionOriginPoint->GetComponentLocation(),
		//DamageProperties.DamageInnerRadius, DamageProperties.DamageOuterRadius,
		//DamageProperties.DamageFalloff, UExplosionDamage::StaticClass(), ActorsToIgnore, InDamageCauser);
	UGameplayStatics::ApplyRadialDamage(this, DamageProperties.BaseDamage,
		ExplosionOriginPoint->GetComponentLocation(), DamageProperties.DamageInnerRadius,
		UExplosionDamage::StaticClass(), ActorsToIgnore);

	TArray<AActor*> OutOverlappingActors;
	ForceAffectingAreaCollision->GetOverlappingActors(OutOverlappingActors);
	for (int i = 0; i < OutOverlappingActors.Num(); i++) {
		if (OutOverlappingActors[i] == nullptr) continue;
		if (IPhysicsActorInterface* PAI = Cast<IPhysicsActorInterface>(OutOverlappingActors[i])) {
			if (PAI && PAI->GetPhysicsComponent()) {
				FVector Direction = OutOverlappingActors[i]->GetActorLocation() - ExplosionOriginPoint->GetComponentLocation();
				Direction = Direction / FVector(FMath::Abs(Direction.X), FMath::Abs(Direction.Y), FMath::Abs(Direction.Z));
				PAI->GetPhysicsComponent()->AddImpulse(Direction * ForceMagnitude);
			}
		}
	}

	OnExplosion();
}

void AExplosive::OnExplosion()
{
	SpawnLoot();

	// Destroy
	Destroy();
}

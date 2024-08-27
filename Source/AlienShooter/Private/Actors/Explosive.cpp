// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.



#include "Actors/Explosive.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

AExplosive::AExplosive()
{
	PrimaryActorTick.bCanEverTick = false;

	ExplosiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosiveMesh"));
	ExplosiveMesh->SetupAttachment(GetRootComponent());

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(ExplosiveMesh);

	ExplosionOriginPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionOriginPoint"));
	ExplosionOriginPoint->SetupAttachment(ExplosiveMesh);
}

void AExplosive::BeginPlay()
{
}

float AExplosive::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser)
{
	// Spawn effects
	UNiagaraSystem* NiagaraSystem = nullptr;
	if (!ExplosionParticleSystem.Get())
		ExplosionParticleSystem.LoadSynchronous();
	NiagaraSystem = ExplosionParticleSystem.Get();
	if (NiagaraSystem)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(),
			GetActorRotation(), GetActorScale3D(), true, true);

	// Apply damage to overlapping characters
	TArray<AActor*> FoundActors;
	AreaSphere->GetOverlappingActors(FoundActors, FilterClass);

	TArray<AActor*> ActorsToIgnore;
	for (auto Character : FoundActors)
	{
		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), DamageProperties.BaseDamage,
			DamageProperties.MinimumDamage, ExplosionOriginPoint->GetComponentLocation(),
			DamageProperties.DamageInnerRadius, DamageProperties.DamageOutherRadius,
			DamageProperties.DamageFalloff, UDamageType::StaticClass(), ActorsToIgnore, InDamageCauser);
	}

	// Destroy mesh
	ExplosiveMesh->SetVisibility(false);
	ExplosiveMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	ExplosiveMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// TODO: Destroy after slight delay so that fx can be noticed

	return 100000.0f;
}

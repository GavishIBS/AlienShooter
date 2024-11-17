// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/ProjectileBullet.h"
#include "Interfaces/HitInterface.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectileBullet::AProjectileBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(static_cast<USceneComponent*>(BoxCollision));

	Tracer_FX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Tracer"));
	Tracer_FX->SetupAttachment(BulletMesh);
}

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBullet::OnBulletHit(AActor* HitActor, const FVector& ImpactPoint)
{
	if (!HitActor) return;

	TArray<FName> ActorTags = HitActor->Tags;
	for (auto It = ProjectileBulletData.HitParticles.CreateIterator(); It; ++It)
	{
		// Check if the tag exists directly in the ActorTags array
		if (ActorTags.Contains(It.Key()))
		{
			// Spawn particle if the value exists
			if (It.Value())
				UGameplayStatics::SpawnEmitterAtLocation(this, It.Value(), ImpactPoint);
			return; // Exit the loop
		}
	}
}

/*
void AProjectileBullet::Init(const float& InBulletHealth, const float& InDamage, const float& InDamage_RandomizationFactor)
{
	Super::Init(InBulletHealth, InDamage, InDamage_RandomizationFactor);

	ProjectileProperties.BulletHealth = InBulletHealth;
	ProjectileProperties.Damage = InDamage;
	ProjectileProperties.Damage_RandomizationFactor = InDamage_RandomizationFactor;
}
*/

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 24, FColor::Cyan, false, 10.f);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Name: %s"), *OtherActor->GetName()));
	if (IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor))
	{
		ProjectileProperties.BulletHealth -= HitInterface->GetHit(Hit.ImpactPoint, ProjectileProperties.Damage,
			ProjectileProperties.Damage_RandomizationFactor, WeaponOwner);
		OnBulletHit(Hit.GetActor(), Hit.ImpactPoint);
	}
	DestroyWrapper();
}

void AProjectileBullet::SetInUse(const bool InValue)
{
	Super::SetInUse(InValue);
	if (InValue)
		Tracer_FX->Activate();
	else Tracer_FX->Deactivate();
}

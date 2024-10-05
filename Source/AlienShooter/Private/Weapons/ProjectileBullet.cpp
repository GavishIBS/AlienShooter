// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/ProjectileBullet.h"
#include "Interfaces/HitInterface.h"
#include "Particles/ParticleSystemComponent.h"


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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Name: %s"), *OtherActor->GetName()));
	if (IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor))
	{
		ProjectileProperties.BulletHealth -= HitInterface->GetHit(Hit.ImpactPoint, ProjectileProperties.Damage,
			ProjectileProperties.Damage_RandomizationFactor, WeaponOwner);
	}
	Destroy();
}

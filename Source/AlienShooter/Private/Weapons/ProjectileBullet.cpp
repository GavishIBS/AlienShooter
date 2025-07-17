// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/ProjectileBullet.h"
#include "Interfaces/HitInterface.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SurfaceTypeDefs.h"

AProjectileBullet::AProjectileBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(static_cast<USceneComponent*>(BoxCollision));
	BulletMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Tracer_FX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Tracer"));
	Tracer_FX->SetupAttachment(BulletMesh);
}

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBullet::OnBulletHit(AActor* HitActor, const FVector& ImpactPoint, UPhysicalMaterial* PhysMat)
{
	if (!HitActor) return;

	if (PhysMat) {
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMat);

		if (ProjectileBulletData.HitEffects.Contains(SurfaceType)) {
			const FBulletHitEffectData& EffectData = ProjectileBulletData.HitEffects[SurfaceType];

			if (EffectData.HitParticle)
				UGameplayStatics::SpawnEmitterAtLocation(this, EffectData.HitParticle, ImpactPoint);

			if (EffectData.HitSound)
				UGameplayStatics::PlaySoundAtLocation(this, EffectData.HitSound, ImpactPoint);
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
	// DEBUG
	//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 24, FColor::Cyan, false, 10.f);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Name: %s"), *OtherActor->GetName()));
	
	if (IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor))
	{
		ProjectileProperties.BulletHealth -= HitInterface->GetHit(Hit.ImpactPoint, ProjectileProperties.Damage,
			ProjectileProperties.Damage_RandomizationFactor, WeaponOwner, UDamageType::StaticClass());
	}
	OnBulletHit(Hit.GetActor(), Hit.ImpactPoint, Hit.PhysMaterial.Get());
	DestroyWrapper();
}

void AProjectileBullet::SetInUse(const bool InValue)
{
	Super::SetInUse(InValue);
	if (InValue)
		Tracer_FX->Activate();
	else Tracer_FX->Deactivate();
}

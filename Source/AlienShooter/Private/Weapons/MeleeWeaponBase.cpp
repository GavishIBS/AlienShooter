// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/MeleeWeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Damage/MeleeAttackDamage.h"
#include "Kismet/GameplayStatics.h"

AMeleeWeaponBase::AMeleeWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeWeaponMesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeWeaponBase::StartAttackTrace()
{
	DamageDealt_ThisAttack = 0.f;
	GetWorldTimerManager().SetTimer(AttackTraceTimerHandle, this, &AMeleeWeaponBase::PerformAttackTrace, AttackTraceRate, true, 0.f);
}

void AMeleeWeaponBase::StopAttackTrace()
{
	GetWorldTimerManager().ClearTimer(AttackTraceTimerHandle);
}

void AMeleeWeaponBase::PerformAttackTrace()
{
	const FVector Start = Mesh->GetSocketLocation(AttackTraceStart_SocketName);
	const FVector End = Mesh->GetSocketLocation(AttackTraceEnd_SocketName);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	FHitResult OutHit;
	UKismetSystemLibrary::SphereTraceSingle(this, Start, End, TraceRadius, ETraceTypeQuery::TraceTypeQuery1, false,
		ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true);

	if (OutHit.bBlockingHit) {
		if (IHitInterface* HitInterface = Cast<IHitInterface>(OutHit.GetActor())) {
			DamageDealt_ThisAttack += HitInterface->GetHit(OutHit.ImpactPoint, DamagePerTrace, DamagePerTrace_Randomization ,GetOwner(), UMeleeAttackDamage::StaticClass());
		}
		if (DamageDealt_ThisAttack > MaxDamagePerAttack) {
			StopAttackTrace();
		}

		{
			OutHit.PhysMaterial.Get();

			if (!OutHit.GetActor()) return;
			if (OutHit.PhysMaterial.Get()) {
				EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(OutHit.PhysMaterial.Get());

				if (HitEffects.Contains(SurfaceType)) {
					const FBulletHitEffectData& EffectData = HitEffects[SurfaceType];

					if (EffectData.HitParticle)
						UGameplayStatics::SpawnEmitterAtLocation(this, EffectData.HitParticle, OutHit.ImpactPoint);

					if (EffectData.HitSound)
						UGameplayStatics::PlaySoundAtLocation(this, EffectData.HitSound, OutHit.ImpactPoint);
				}
			}
		}
	}
}
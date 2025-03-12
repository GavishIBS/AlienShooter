// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Interfaces/ExplodeInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Damage/ExplosionDamage.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/PhysicsActorInterface.h"

// Add default functionality here for any IExplodeInterface functions that are not pure virtual.

// TODO: This function should take in niagara particle system instead of cascade particle system
void IExplodeInterface::Explode(AActor* ThisObject, UParticleSystem* InExplosionParticleSystem, const FDamageProperties& InDamageProperties,
	const FVector& Loc, const FRotator& Rot, const TArray<AActor*> InActorsToIgnore)
{
	if (!ThisObject) return;

	/*
	// Spawn effects
	UNiagaraSystem* NiagaraSystem = nullptr;
	if (!ExplosionParticleSystem.Get())
		ExplosionParticleSystem.LoadSynchronous();
	NiagaraSystem = ExplosionParticleSystem.Get();
	if (NiagaraSystem)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(),
			GetActorRotation(), GetActorScale3D(), true, true);
	*/

	if (InExplosionParticleSystem)
		UGameplayStatics::SpawnEmitterAtLocation(ThisObject->GetWorld(), InExplosionParticleSystem, Loc, Rot, true);

	UGameplayStatics::ApplyRadialDamage(ThisObject, InDamageProperties.BaseDamage, Loc, InDamageProperties.DamageInnerRadius,
		UExplosionDamage::StaticClass(), InActorsToIgnore);

	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::SphereTraceMulti(ThisObject, Loc, Loc,
		InDamageProperties.DamageInnerRadius, ETraceTypeQuery::TraceTypeQuery1, false, InActorsToIgnore, EDrawDebugTrace::None,
		OutHits, true);

	TArray<AActor*> OutOverlappingActors;
	for (auto Hit : OutHits) {
		if (Hit.bBlockingHit) {
			OutOverlappingActors.Add(Hit.GetActor());
		}
	}

	for (int i = 0; i < OutOverlappingActors.Num(); i++) {
		if (OutOverlappingActors[i] == ThisObject) continue;
		if (OutOverlappingActors[i] == nullptr) continue;
		if (IHitInterface* HI = Cast<IHitInterface>(OutOverlappingActors[i])) {
			HI->GetHit(OutOverlappingActors[i]->GetActorLocation(), InDamageProperties.BaseDamage, 1.f, ThisObject, UExplosionDamage::StaticClass());
		}
		if (IPhysicsActorInterface* PAI = Cast<IPhysicsActorInterface>(OutOverlappingActors[i])) {
			if (PAI && PAI->GetPhysicsComponent()) {
				FVector Direction = OutOverlappingActors[i]->GetActorLocation() - Loc;
				Direction = Direction / FVector(FMath::Abs(Direction.X), FMath::Abs(Direction.Y), FMath::Abs(Direction.Z));
				PAI->GetPhysicsComponent()->AddImpulse(Direction * InDamageProperties.ForceMag);
			}
		}
	}
}

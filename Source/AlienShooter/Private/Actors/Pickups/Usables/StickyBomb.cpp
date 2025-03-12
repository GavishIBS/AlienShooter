// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/Usables/StickyBomb.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Damage/ExplosionDamage.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/ExplodeInterface.h"

AStickyBomb::AStickyBomb()
{
}

void AStickyBomb::Explode(AActor* ThisObject, UParticleSystem* InExplosionParticleSystem, const FDamageProperties& InDamageProperties,
	const FVector& Loc, const FRotator& Rot, const TArray<AActor*> InActorsToIgnore)
{
	IExplodeInterface::Explode(ThisObject, InExplosionParticleSystem, InDamageProperties, Loc, Rot, InActorsToIgnore);
}

void AStickyBomb::BeginPlay()
{
	Super::BeginPlay();
}

bool AStickyBomb::CanUse(ACharacter* User, FHitResult& HitResult)
{
	return Super::CanUse(User, HitResult);
}

AUsableActor* AStickyBomb::UseItem(ACharacter* User)
{
	FHitResult HitResult;
	if (!CanUse(User, HitResult)) return 0;

	SetActorLocation(HitResult.ImpactPoint);
	const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(HitResult.ImpactPoint, GetActorLocation() + HitResult.ImpactNormal);
	SetActorRotation(NewRotation);
	SetActorScale3D({1.f, 1.f, 1.f});

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	AttachToActor(HitResult.GetActor(), AttachmentTransformRules);

	StartExplosionTimer();

	return this;
}

void AStickyBomb::StartExplosionTimer()
{
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AStickyBomb::ExplodeWrapper, ExplosionDelay, false, ExplosionDelay);
}

void AStickyBomb::ExplodeWrapper()
{
	/*
	if (ExplosionParticleSystem)
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticleSystem, GetActorLocation(), GetActorRotation(), true);

	UGameplayStatics::ApplyRadialDamage(this, DamageProperties.BaseDamage,
		Mesh->GetComponentLocation(), DamageProperties.DamageInnerRadius,
		UExplosionDamage::StaticClass(), TArray<AActor*>());

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("Explosion"));
	*/

	Explode(this, ExplosionParticleSystem, DamageProperties, GetActorLocation(), GetActorRotation(), TArray<AActor*>());
	Destroy();
}

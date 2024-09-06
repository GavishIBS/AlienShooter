// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/Usables/StickyBomb.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Damage/ExplosionDamage.h"

AStickyBomb::AStickyBomb()
{
}

void AStickyBomb::BeginPlay()
{
	Super::BeginPlay();
}

bool AStickyBomb::CanUse(ACharacter* User, FHitResult& HitResult)
{
	return Super::CanUse(User, HitResult);
}

bool AStickyBomb::UseItem(ACharacter* User)
{
	FHitResult HitResult;
	if (!CanUse(User, HitResult)) return false;

	SetActorLocation(HitResult.ImpactPoint);
	// TODO: Set rotation

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	AttachToActor(HitResult.GetActor(), AttachmentTransformRules);

	StartExplosionTimer();

	return true;
}

void AStickyBomb::StartExplosionTimer()
{
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AStickyBomb::Explode, ExplosionDelay, false, ExplosionDelay);
}

void AStickyBomb::Explode()
{
	UGameplayStatics::ApplyRadialDamage(this, DamageProperties.BaseDamage,
		Mesh->GetComponentLocation(), DamageProperties.DamageInnerRadius,
		UExplosionDamage::StaticClass(), TArray<AActor*>());

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("Explosion"));

	Destroy();
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/SpecialEffectProjectileBullet.h"
#include "Interfaces/HitInterface.h"

ASpecialEffectProjectileBullet::ASpecialEffectProjectileBullet()
{

}

void ASpecialEffectProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ASpecialEffectProjectileBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Name: %s"), *OtherActor->GetName()));
	if (IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor))
	{
		//ProjectileProperties.BulletHealth -= HitInterface->GetSpecialHit(SpecialEffectData);
	    HitInterface->GetSpecialHit(Hit.ImpactPoint, SpecialEffectData, ProjectileProperties.Damage_RandomizationFactor, WeaponOwner);
	}
	Destroy();
}

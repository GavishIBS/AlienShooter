// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/BreakableWall.h"
#include "Damage/ExplosionDamage.h"

ABreakableWall::ABreakableWall()
{
}

void ABreakableWall::BeginPlay()
{
	Super::BeginPlay();
	InitCallbacks();
}

void ABreakableWall::InitCallbacks()
{
	OnTakeAnyDamage.AddDynamic(this, &ABreakableWall::OnTakeDamageCallback);
}

void ABreakableWall::OnTakeDamageCallback(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	GetHit(GetActorLocation(), Damage, 0.f, DamageCauser, DamageType->GetClass());
}

float ABreakableWall::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType)
{
	if (DamageTypeClass.Get() != UExplosionDamage::StaticClass()) return 0;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString("Apply Damage"));
	ReplaceMesh();
	return Damage;
}
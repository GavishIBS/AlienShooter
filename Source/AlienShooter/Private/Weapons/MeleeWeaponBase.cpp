// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/MeleeWeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"

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

	}
}

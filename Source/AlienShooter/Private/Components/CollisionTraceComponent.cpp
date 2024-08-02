// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Components/CollisionTraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UCollisionTraceComponent::UCollisionTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCollisionTraceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCollisionTraceComponent::InitComponent(USkeletalMeshComponent* InMesh)
{
	OwnerMesh = InMesh;
}

FHitResult UCollisionTraceComponent::DoTraceSingle(const TArray<AActor*>& ActorsToIgnore, const int Index)
{
	FHitResult ReturnValue;
	if (!OwnerMesh) return ReturnValue;
	//if (TraceSets.Num() <= Index) return ReturnValue;
	
	if (TraceShape == ETS_Sphere)
	{
		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(GetOwner());
		const FVector Start = OwnerMesh->GetSocketLocation(TraceSet.TraceStartSocketName);
		const FVector End = OwnerMesh->GetSocketLocation(TraceSet.TraceEndSocketName);
		UKismetSystemLibrary::SphereTraceSingle(OwnerMesh, Start, End, TraceRadius, TraceTypeQuery1, false,
			ActorToIgnore, EDrawDebugTrace::ForDuration, ReturnValue, true);
	}
	
	return ReturnValue;
}

TArray<FHitResult> UCollisionTraceComponent::DoTraceMulti(const TArray<AActor*>& ActorsToIgnore, const int Index)
{
	return TArray<FHitResult>();
}


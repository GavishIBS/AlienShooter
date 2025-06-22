// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Utils/MeshReplacer.h"
#include "Engine/StaticMeshActor.h"

AMeshReplacer::AMeshReplacer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMeshReplacer::BeginPlay()
{
	Super::BeginPlay();
}

void AMeshReplacer::Replace()
{
	if (ActorToBePlacedClasses.Num() == 0) return;
	UWorld* World = GetWorld();
	if (!World) return;
	for (auto& SMA : MeshesToReplace) {
		if (!SMA) continue;
		int Index = FMath::RandRange(0, ActorToBePlacedClasses.Num() - 1);
		TSubclassOf<AActor> ActorToBePlacedClass;
		ActorToBePlacedClass = ActorToBePlacedClasses[Index];
		if (!ActorToBePlacedClass) return;
		FTransform Transform = SMA->GetActorTransform();
		AActor* SpawnedActor = World->SpawnActor<AActor>(ActorToBePlacedClass);
		if (SpawnedActor) {
			SpawnedActor->SetActorTransform(Transform);
			SpawnedActor->SetActorScale3D(SpawnedActor->GetActorScale3D() * ScalingFactor);
			SMA->Destroy();
		}
	}
	MeshesToReplace.Empty();
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/PhysicsStaticMeshActor.h"

APhysicsStaticMeshActor::APhysicsStaticMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);
}

UPrimitiveComponent* APhysicsStaticMeshActor::GetPhysicsComponent()
{
	return (UPrimitiveComponent*)Mesh;
}

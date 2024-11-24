// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/ReplaceableWall.h"

AReplaceableWall::AReplaceableWall()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

void AReplaceableWall::BeginPlay()
{
	Super::BeginPlay();
}

void AReplaceableWall::ReplaceMesh()
{
	if (!ReservedMesh) return;

	UStaticMesh* LastMeshAsset = Mesh->GetStaticMesh();
	Mesh->SetStaticMesh(ReservedMesh);
	OnMeshChanged();
	if (bReverseable)
		ReservedMesh = LastMeshAsset;
	else ReservedMesh = nullptr;
}

void AReplaceableWall::OnMeshChanged()
{
}

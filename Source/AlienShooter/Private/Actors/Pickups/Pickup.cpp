// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/Pickup.h"
#include "Components/SphereComponent.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(Mesh);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	InitCallbacks();
	if (!bTick) SetActorTickEnabled(false);
}

void APickup::Tick(float DeltaTime)
{
	if (Mesh)
	{
		FRotator NewRotation = Mesh->GetComponentRotation();
		NewRotation.Yaw += RotationSpeed;
		Mesh->SetWorldRotation(NewRotation);
	}
}


void APickup::InitCallbacks()
{
	if (OverlapSphere)
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnSphereOverlap);
}

void APickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PostPickup(OtherActor))
		Destroy();
}

bool APickup::PostPickup(AActor* Caller)
{
	return true;
}

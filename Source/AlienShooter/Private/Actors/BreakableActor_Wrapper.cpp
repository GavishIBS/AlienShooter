// // Copyright (c) 2024 Gavish Bansal// // All rights reserved.


#include "Actors/BreakableActor_Wrapper.h"
#include "Components/StaticMeshComponent.h"
#include "Actors/BreakableActor.h"

ABreakableActor_Wrapper::ABreakableActor_Wrapper()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BreakableMesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

float ABreakableActor_Wrapper::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType)
{
	if (!BreakableActorClass)
	{
		Destroy();
		return 0.f;
	}

	ABreakableActor* BA = GetWorld()->SpawnActor<ABreakableActor>(BreakableActorClass, GetActorTransform());
	if (BA)
	{
		BA->GetHit(HitLocation, Damage, RandomizationFactor, InDamageCauser, DamageTypeClass, OutGetHitReturnType);
	}
	Destroy();

	return 0.f;
}

void ABreakableActor_Wrapper::BeginPlay()
{
	Super::BeginPlay();
	if (!BreakableActorClass) Destroy();
}

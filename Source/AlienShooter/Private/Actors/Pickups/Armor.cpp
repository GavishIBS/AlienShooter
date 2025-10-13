// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/Armor.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AArmor::AArmor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AArmor::BeginPlay()
{
	Super::BeginPlay();

	/*
	ArmorPiecesMeshComponents.SetNum(0);
	for (int i = 0; i < ArmorPieceMeshes.Num(); i++)
	{
		ArmorPiecesMeshComponents.AddUnique(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmorPieceSMComponent")));
		ArmorPiecesMeshComponents[i]->SetStaticMesh(ArmorPieceMeshes[i]);
		ArmorPiecesMeshComponents[i]->SetVisibility(false);
	}
	*/

	ArmorPiecesMeshComponents.Empty();
	for (int32 i = 0; i < ArmorPieceMeshes.Num(); i++)
	{
		UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(this);
		if (NewMeshComp)
		{
			NewMeshComp->RegisterComponent();
			NewMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			NewMeshComp->SetStaticMesh(ArmorPieceMeshes[i]);
			NewMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			NewMeshComp->SetVisibility(false);

			ArmorPiecesMeshComponents.Add(NewMeshComp);
		}
	}
}

void AArmor::InitCallbacks()
{
	Super::InitCallbacks();
}

void AArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArmor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;

	if (UAttributeComponent* AttributeComponent = OtherActor->FindComponentByClass<UAttributeComponent>())
	{
		if (AttributeComponent->CanTakeArmor(ArmorColorCode))
		{
			if (AttributeComponent->TakeArmor(this))
			{
				ClearDespawnTimer();
			}
			Mesh->DestroyComponent();
			OverlapSphere->SetGenerateOverlapEvents(false);
			//Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex,
				//bFromSweep, SweepResult);
		}
	}
}

void AArmor::AbsorbDamage(const float InDamage)
{
	ArmorProperties.CurrentArmorHealth = FMath::Clamp(ArmorProperties.CurrentArmorHealth - InDamage, 0.f, ArmorProperties.MaxArmorHealth);
}

void AArmor::Break()
{
	if (DestructionParticleSystem)
	{
		//UGameplayStatics::SpawnEmitterAtLocation(this, DestructionParticleSystem, OverlapSphere->GetComponentLocation(), OverlapSphere->GetComponentRotation(), true);
		UGameplayStatics::SpawnEmitterAtLocation(this, DestructionParticleSystem, GetActorLocation(), GetActorRotation(), true);
	}
	Destroy();
}

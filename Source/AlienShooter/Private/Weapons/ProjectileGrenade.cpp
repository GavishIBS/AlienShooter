// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/ProjectileGrenade.h"
#include "Kismet/GameplayStatics.h"
#include "Damage/ExplosionDamage.h"
#include "Components/StaticMeshComponent.h"
#include "AI/Pawns/BaseAICharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"
#include "Kismet/KismetSystemLibrary.h"

AProjectileGrenade::AProjectileGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	GrenadeMesh->SetupAttachment(static_cast<USceneComponent*>(BoxCollision));
}

void AProjectileGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (ExplosionParticleSystem)
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticleSystem, GetActorLocation(), GetActorRotation(), true);

	/*
	TArray<AActor*> ActorsToIgnore;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageProperties.BaseDamage, GetActorLocation(),
		DamageProperties.DamageInnerRadius, UDamageType::StaticClass(), ActorsToIgnore);
	*/
	
	



    // TEMPRARY BEGIN

    FVector Location = GetActorLocation(); // Set the center of the sphere

    // Array to store overlapping actors
    TArray<AActor*> OverlappingActors;

    // Perform the sphere overlap
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        Location,
        DamageProperties.DamageInnerRadius,
        TArray<TEnumAsByte<EObjectTypeQuery>>(), // You can specify object types here
        AActor::StaticClass(), // You can filter for specific actor classes here
        TArray<AActor*>(), // Ignore actors (optional)
        OverlappingActors // Output array
    );

    // Loop through the overlapping actors
    for (AActor* Actor : OverlappingActors)
    {
        if (Actor)
        {
            UGameplayStatics::ApplyDamage(Actor, DamageProperties.BaseDamage, nullptr, nullptr, UExplosionDamage::StaticClass());
            UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *Actor->GetName());
        }
    }

    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageProperties.DamageInnerRadius, 24, FColor::Orange, false, 5.f); // DEBUG


    // TEMPORARY END






	Destroy();
}

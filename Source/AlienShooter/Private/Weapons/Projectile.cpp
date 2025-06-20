// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	BoxCollision->bReturnMaterialOnMove = true;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (BoxCollision)
		BoxCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::Tick(float DeltaTime)
{
}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	GetWorldTimerManager().ClearTimer(ProjectileTimerHandle);
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

/*
void AProjectile::Init(const float& InBulletHealth, const float& InDamage, const float& InDamage_RandomizationFactor)
{
}
*/

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
}

void AProjectile::DestroyWrapper()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString("Destroy wrapper called!"));
	if (WasSpawnedByPool())
		SetInUse(false);
	else {
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString("Destroying projectile from world!"));
		Destroy();
	}
}

void AProjectile::UpdateProjectile()
{
	ProjectileMovementComponent->InitialSpeed = FMath::Clamp(ProjectileMovementComponent->InitialSpeed + ProjectileProperties.SpeedAdditive,
		ProjectileProperties.MinimumProjectileSpeed, ProjectileMovementComponent->MaxSpeed);

	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	
	const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	SetActorRotation(NewRotation);

	//if (GetActorLocation() == TargetLocation)
	if (GetActorLocation().Equals(TargetLocation, 50.f))
	{
		//TargetLocation += ProjectileMovementComponent->Velocity.GetSafeNormal() * 20;
		//TargetLocation.Z -= 1000.f;
		TargetLocation += GetActorForwardVector() * 500.f;
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AProjectile::DestroyWrapper, 3.f, false, 3.f);
	}

	//DrawDebugSphere(GetWorld(), TargetLocation, 10.f, 24, FColor::Emerald, false, 10.f);
	//DrawDebugLine(GetWorld(), TargetLocation, TargetLocation + (Direction * 5), FColor::Red, false, 10.f);
}

void AProjectile::SetInUse(const bool InValue)
{
	Super::SetInUse(InValue);
	if (IsInUse())
	{
		ProjectileMovementComponent->Activate();
		GetWorldTimerManager().SetTimer(ProjectileTimerHandle, this, &AProjectile::UpdateProjectile, ProjectileUpdateRate, true, 0.f);

	}
	else {
		ProjectileMovementComponent->Deactivate();
		GetWorldTimerManager().ClearTimer(ProjectileTimerHandle);
	}
}

void AProjectile::SetTargetLocation(const FVector& TargetLoc)
{
	TargetLocation = TargetLoc;

	float DistanceZ = FMath::Abs(TargetLoc.Z - GetActorLocation().Z);
	if (DistanceZ > ProjectileProperties.Limit_DistanceZ)
	{
		if (GetActorLocation().Z < TargetLoc.Z)
		{
			TargetLocation.Z = GetActorLocation().Z;
			TargetLocation.Z += ProjectileProperties.Limit_DistanceZ;
		}
	}
}

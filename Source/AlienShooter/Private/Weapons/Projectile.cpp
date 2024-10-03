// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/HitInterface.h"

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
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	//FVector Vel = Direction * ProjectileMovementComponent->InitialSpeed;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%f : %f"), Vel.X, Vel.Y));

}

void AProjectile::Destroyed()
{
	Super::Destroyed();
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

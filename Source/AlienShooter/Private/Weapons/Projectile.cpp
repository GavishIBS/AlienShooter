// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/HitInterface.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (BoxCollision)
		BoxCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::Destroyed()
{
	Super::Destroyed();
}

void AProjectile::Init(const float& InBulletHealth, const float& InDamage, const float& InDamage_RandomizationFactor)
{
	ProjectileProperties.BulletHealth = InBulletHealth;
	ProjectileProperties.Damage = InDamage;
	ProjectileProperties.Damage_RandomizationFactor = InDamage_RandomizationFactor;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	if (IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor))
		ProjectileProperties.BulletHealth -= HitInterface->GetHit(Hit.ImpactPoint, ProjectileProperties.Damage, ProjectileProperties.Damage_RandomizationFactor, WeaponOwner);
	Destroy();
}

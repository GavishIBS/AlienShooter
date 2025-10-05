// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/Attacks/EnemyAttackProjectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CustomCollisionTypes.h"
#include "Characters/ShooterCharacter.h"

AEnemyAttackProjectile::AEnemyAttackProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Block);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(static_cast<USceneComponent*>(BoxCollision));
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AEnemyAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	BindCallbacks();
}

void AEnemyAttackProjectile::Destroyed()
{
	Super::Destroyed();
}

void AEnemyAttackProjectile::BindCallbacks()
{
	if (BoxCollision)
	{
		BoxCollision->OnComponentHit.AddDynamic(this, &AEnemyAttackProjectile::OnHit);
	}
}

void AEnemyAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyAttackProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("OnHit Called")));

	if (OtherActor == this || OtherActor->GetClass() == this->GetClass()) {
		Destroy();
		return;
	}

	if (AShooterCharacter* SC = Cast<AShooterCharacter>(OtherActor))
	{
		SC->GetHit(Hit.ImpactPoint, 5.f, 2.f, GetOwner(), UDamageType::StaticClass());
	}
	OnAttackProjectileHit(OtherActor, HitComponent);
	Destroy();
}

void AEnemyAttackProjectile::OnAttackProjectileHit(AActor* HitActor, UPrimitiveComponent* HitComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("HitActorName: %s"), *HitActor->GetName()));
}

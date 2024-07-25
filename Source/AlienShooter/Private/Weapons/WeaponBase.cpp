// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/WeaponBase.h"
#include "Characters/ShooterCharacter.h"
#include "Components/SphereComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(GetRootComponent());
	
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(OverlapSphere);

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticeMesh"));
	WeaponStaticMesh->SetupAttachment(OverlapSphere);

	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponSkeletalMesh->SetSimulatePhysics(false);

	WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponStaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponStaticMesh->SetSimulatePhysics(false);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	Init_Callbacks();
}

void AWeaponBase::Init_Callbacks()
{
	if (OverlapSphere)
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnBeginOverlap);
	OnWeaponPicked.AddDynamic(this, &AWeaponBase::OnWeaponPickedCallback);
}

void AWeaponBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (const AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor))
	{
		ShooterCharacter->PickupWeapon(this);
	}
}

void AWeaponBase::OnWeaponPickedCallback()
{
	OverlapSphere->SetGenerateOverlapEvents(false);
}

/*
bool AWeaponBase::TakeWeapon(const ACharacter* InstigatorCharacter)
{
	if (!InstigatorCharacter) return false;

	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, false);
	this->AttachToComponent(InstigatorCharacter->GetMesh(), AttachmentTransformRules,
		WeaponProperties.InHandAttachmentSocketName);
	return false;
}*/

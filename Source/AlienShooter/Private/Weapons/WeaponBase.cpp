// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/WeaponBase.h"
#include "Characters/ShooterCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Weapons/BulletShell.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(GetRootComponent());
	
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(OverlapSphere);
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponSkeletalMesh->SetSimulatePhysics(false);

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticeMesh"));
	WeaponStaticMesh->SetupAttachment(OverlapSphere);
	WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponStaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponStaticMesh->SetSimulatePhysics(false);
	
	WeaponMuzzle_S = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponMuzzle_S"));
	WeaponMuzzle_S->SetupAttachment(WeaponStaticMesh);

	MuzzleDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleDirection"));
	MuzzleDirection->SetupAttachment(OverlapSphere);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	Init_Callbacks();
	
	if (WeaponProperties.bIsSkeletalMesh)
	{
		WeaponStaticMesh->DestroyComponent();
		WeaponMuzzle_S->DestroyComponent();
	}
}

void AWeaponBase::Init_Callbacks()
{
	if (OverlapSphere)
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnBeginOverlap);
	OnWeaponPicked.AddDynamic(this, &AWeaponBase::OnWeaponPickedCallback);
	OnWeaponSelected.AddDynamic(this, &AWeaponBase::OnWeaponSelectedCallback);
	OnWeaponFirePressed.AddDynamic(this, &AWeaponBase::OnWeaponFirePressedCallback);
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
	OwnerAsShooterCharacter = Cast<AShooterCharacter>(GetOwner());
}

void AWeaponBase::OnWeaponSelectedCallback()
{
	OverlapSphere->SetGenerateOverlapEvents(false);
	OwnerAsShooterCharacter = Cast<AShooterCharacter>(GetOwner());
}

void AWeaponBase::OnWeaponFirePressedCallback()
{
	if (CanFire())
	{
		WeaponProperties.DeductAmmo(1);
		Fire();
	} else WeaponFiredFailed();
}

bool AWeaponBase::CanFire() const
{
	if (GetTotalAmmoAvailable() <= 0) return false;
	return true;
}

void AWeaponBase::Fire()
{
	OnWeaponFired.Broadcast();

	if (WeaponData.BulletShellClass)
		SpawnBulletShell();
}

void AWeaponBase::WeaponFiredFailed()
{
}

void AWeaponBase::SpawnBulletShell() const
{
	if (!GetWorld()) return;

	const FTransform EjectionSocketTransform = WeaponProperties.bIsSkeletalMesh ?
		WeaponSkeletalMesh->GetSocketTransform(WeaponProperties.ShellEjectionSocketName) :
		WeaponStaticMesh->GetSocketTransform(WeaponProperties.ShellEjectionSocketName);

	GetWorld()->SpawnActor<ABulletShell>(WeaponData.BulletShellClass, EjectionSocketTransform);
}

FTransform AWeaponBase::GetWeaponMuzzleTransform() const
{
	if (WeaponProperties.bIsSkeletalMesh && WeaponSkeletalMesh)
		return WeaponSkeletalMesh->GetSocketTransform(WeaponProperties.WeaponMuzzleSocketName, RTS_World);

	if (WeaponStaticMesh)
		return WeaponMuzzle_S->GetComponentTransform();

	return FTransform();
}

FTransform AWeaponBase::GetLeftHandPlacementTransform() const
{
	if (WeaponProperties.bIsSkeletalMesh)
		return WeaponSkeletalMesh->GetSocketTransform(WeaponProperties.WeaponLeftHandPlacementSocketName, RTS_World);
	return WeaponStaticMesh->GetSocketTransform(WeaponProperties.WeaponLeftHandPlacementSocketName, RTS_World);
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

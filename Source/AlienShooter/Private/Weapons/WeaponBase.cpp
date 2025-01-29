// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/WeaponBase.h"
#include "Characters/ShooterCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Weapons/BulletShell.h"
#include "AnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"

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

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserMesh"));
	LaserMesh->SetupAttachment(WeaponStaticMesh);
	LaserMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AWeaponBase::TryReload()
{
	if (WeaponProperties.AmmoAvailable > 0) {
		if (WeaponProperties.bIsSkeletalMesh) {
			checkf(WeaponSkeletalMesh, TEXT("bIsSkeletalMesh is set to false but, WeaponSkeletalMesh is not valid on reload"));
			AnimationLibrary::PlayAnimationMontage(WeaponSkeletalMesh, WeaponAnimationData.WeaponReloadAnimations);
		}
		OnWeaponReload.Broadcast();
		UpdateMagazine();
	} else return false;
	return true;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	Init_Callbacks();
	
	if (WeaponProperties.bIsSkeletalMesh)
	{
		WeaponStaticMesh->DestroyComponent();
		WeaponMuzzle_S->DestroyComponent();
		LaserMesh->SetupAttachment(WeaponSkeletalMesh);
	}
	else {
		WeaponSkeletalMesh->DestroyComponent();
	}
	if (!LaserProperties.bLaser) LaserMesh->DestroyComponent();
	else {
		// Update laser color
		UMaterialInstanceDynamic* DynamicMaterial = LaserMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			const FLinearColor NewEmissiveColor = LaserProperties.LaserColor;
			DynamicMaterial->SetVectorParameterValue(FName("EmissiveColor"), NewEmissiveColor);
		}
	}
}

void AWeaponBase::Init_Callbacks()
{
	if (OverlapSphere)
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnBeginOverlap);
	OnWeaponPicked.AddDynamic(this, &AWeaponBase::OnWeaponPickedCallback);
	OnWeaponSelected.AddDynamic(this, &AWeaponBase::OnWeaponSelectedCallback);
	OnWeaponFirePressed.AddDynamic(this, &AWeaponBase::OnWeaponFirePressedCallback);
	OnDestroyed.AddDynamic(this, &AWeaponBase::OnWeaponDestroyed);
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
		Fire();
	} else WeaponFiredFailed();
}

void AWeaponBase::OnWeaponDestroyed(AActor* DestroyedActor)
{
}

bool AWeaponBase::CanFire() const
{
	if (GetTotalAmmoAvailable() <= 0) return false;
	return true;
}

void AWeaponBase::Fire()
{
	WeaponProperties.DeductAmmo(1);
	
	if (WeaponSkeletalMesh)
	{
		AnimationLibrary::PlayAnimation(WeaponSkeletalMesh, WeaponAnimationData.WeaponFireAnimations);
	}

	if (WeaponData.BulletShellClass)
		SpawnBulletShell();

	OnWeaponFired.Broadcast();
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

void AWeaponBase::UpdateMagazine()
{
	/*
	const int AmmoToFeed = GetTotalAmmoAvailable() >= WeaponProperties.MagazineMaxSize ? WeaponProperties.MagazineMaxSize : GetTotalAmmoAvailable();
	WeaponProperties.AmmoInMagazine = AmmoToFeed;
	SetTotalAmmoAvailable(GetTotalAmmoAvailable() - AmmoToFeed);
	*/
	
	//WeaponProperties.AmmoInMagazine = GetTotalAmmoAvailable() >= WeaponProperties.MagazineMaxSize ? WeaponProperties.MagazineMaxSize : GetTotalAmmoAvailable();
	//SetTotalAmmoAvailable(GetTotalAmmoAvailable() - WeaponProperties.AmmoInMagazine);
	
	WeaponProperties.SetAmmoInMag(GetAmmoInBag() >= WeaponProperties.MagazineMaxSize ? WeaponProperties.MagazineMaxSize : GetAmmoInBag());
	WeaponProperties.SetAmmoInBag(GetAmmoInBag() - WeaponProperties.AmmoInMagazine);
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

void AWeaponBase::UpdateLaser(const FVector& EndLocation, const bool& bHittableAhead)
{
	if (!LaserMesh) return;
	
	const FVector LaserWorldLocation = LaserMesh->GetComponentLocation();

	// Scale correction
	const float Distance = FVector::Distance(LaserWorldLocation, EndLocation);
	FVector NewScale3D = LaserMesh->GetComponentScale();
	if ((LaserProperties.Limit_GlowOnHittableActorsOnly && !bHittableAhead) ||
		Distance < LaserProperties.Limit_DistanceXY ||
		FMath::Abs(EndLocation.Z - LaserWorldLocation.Z) > LaserProperties.Limit_DistanceZ)
	{
		NewScale3D.X = 0.f;
		LaserMesh->SetWorldScale3D(NewScale3D);
		return;
	}
	NewScale3D.X = Distance / 200.f;
	LaserMesh->SetWorldScale3D(NewScale3D);


	// Rotation correction
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(LaserWorldLocation, EndLocation);
	LaserMesh->SetWorldRotation(TargetRotation);
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

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/BulletShell.h"
#include "Kismet/GameplayStatics.h"

ABulletShell::ABulletShell()
{
	PrimaryActorTick.bCanEverTick = false;

	ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShellMesh"));
	ShellMesh->SetupAttachment(GetRootComponent());
	ShellMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ShellMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ShellMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	ShellMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	ShellMesh->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
	ShellMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	ShellMesh->SetSimulatePhysics(true);
	ShellMesh->SetEnableGravity(true);
	ShellMesh->SetNotifyRigidBodyCollision(true);
}

void ABulletShell::BeginPlay()
{
	Super::BeginPlay();
	
	if (ShellMesh)
	{
		ShellMesh->OnComponentHit.AddDynamic(this, &ABulletShell::OnHit);
		ShellMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
	}
}

void ABulletShell::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(),
			GetActorRotation(), 1.f, 1.f, 0.f, SoundAttenuation, SoundConcurrency);

	GetWorldTimerManager().SetTimer(ShellDestroyHandle, this, &ABulletShell::DestroyBulletShell, DestroyDelay, false);
}

void ABulletShell::DestroyBulletShell()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(ShellDestroyHandle);
}

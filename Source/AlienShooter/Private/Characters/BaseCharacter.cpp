// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Characters/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InventoryComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Player");

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 1000.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	CameraOriginalLocation_Rel = ViewCamera->GetRelativeLocation();

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	ViewCamera->SetRelativeLocation(FMath::VInterpTo(ViewCamera->GetRelativeLocation(), CameraTargetLocation_Rel, DeltaTime, 3.f));
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (InventoryComponent)
		InventoryComponent->InitInventory();
}

float ABaseCharacter::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	return IHitInterface::GetHit(HitLocation, Damage, RandomizationFactor, InDamageCauser, DamageTypeClass);
}

void ABaseCharacter::DoCameraShake(const FVector IntensityFactor, const float ShakeTime, const bool bShakeInDir, const FVector Direction)
{
	CameraShakeIntensity = IntensityFactor;
	bShakeInDirection = bShakeInDir;
	ShakeDirection = Direction;
	if (!bShakeInDirection) {
		GetWorldTimerManager().SetTimer(CameraShakeTimerHandle, this, &ABaseCharacter::GenerateRandomCameraLocation, 0.05, true, 0);
	}
	else {
		CameraTargetLocation_Rel = CameraOriginalLocation_Rel + FVector(
			CameraShakeIntensity.X * ShakeDirection.X,
			CameraShakeIntensity.Y * ShakeDirection.Y,
			CameraShakeIntensity.Z * ShakeDirection.Z
		);
	}
	GetWorldTimerManager().SetTimer(CameraShakeStopTimerHandle, this, &ABaseCharacter::StopCameraShake, ShakeTime, false, ShakeTime);
}

void ABaseCharacter::GenerateRandomCameraLocation()
{
	if (!bShakeInDirection) {
		CameraTargetLocation_Rel = CameraOriginalLocation_Rel + FVector(
			FMath::RandRange(-15, 20) * CameraShakeIntensity.X,
			FMath::RandRange(-20, 20) * CameraShakeIntensity.Y,
			FMath::RandRange(-5, 10) * CameraShakeIntensity.Z
		);
	}
	/*
	else {
		CameraTargetLocation_Rel = CameraOriginalLocation_Rel + FVector(
			CameraShakeIntensity.X * ShakeDirection.X,
			CameraShakeIntensity.Y * ShakeDirection.Y,
			CameraShakeIntensity.Z * ShakeDirection.Z
		);
	}
	*/
}

void ABaseCharacter::StopCameraShake()
{
	GetWorldTimerManager().ClearTimer(CameraShakeTimerHandle);
	CameraTargetLocation_Rel = CameraOriginalLocation_Rel;
	GetWorldTimerManager().ClearTimer(CameraShakeStopTimerHandle);
}

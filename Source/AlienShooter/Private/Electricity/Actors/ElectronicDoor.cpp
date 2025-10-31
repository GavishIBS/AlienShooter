// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Electricity/Actors/ElectronicDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

AElectronicDoor::AElectronicDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(GetRootComponent());
	InitialDoorLocation = DoorMesh->GetComponentLocation();

	OpenDoorBoxOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("OpenDoorBoxOverlap"));
	OpenDoorBoxOverlap->SetupAttachment(DoorMesh);

	CloseDoorBoxOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("CloseDoorBoxOverlap"));
	CloseDoorBoxOverlap->SetupAttachment(DoorMesh);
	
	ElectricityIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElectricityIndicatorMesh"));
	ElectricityIndicatorMesh->SetupAttachment(DoorMesh);
}

void AElectronicDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpening)
	{
		FVector Target = InitialDoorLocation + TargetLocationOffset;
		FVector Door_NewLocation = FMath::VInterpTo(DoorMesh->GetComponentLocation(),
			Target, DeltaTime, DoorSpeed);
		DoorMesh->SetWorldLocation(Door_NewLocation);
		OnDoorMovementTick.Broadcast();   // door movement tick
		if (Door_NewLocation == Target)
		{
			bOpening = false;
		}
		return;
	}
	else if (bClosing)
	{
		FVector Target = InitialDoorLocation;
		FVector Door_NewLocation = FMath::VInterpTo(DoorMesh->GetComponentLocation(),
			Target, DeltaTime, DoorSpeed);
		DoorMesh->SetWorldLocation(Door_NewLocation);
		OnDoorMovementTick.Broadcast();   // door movement tick
		if (Door_NewLocation == Target)
		{
			bClosing = false;
		}
		return;
	}
}

void AElectronicDoor::StartForceKeepOpen()
{
	OpenDoorBoxOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CloseDoorBoxOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OpenDoor();

}

void AElectronicDoor::EndForceKeepOpen()
{
	CloseDoor();
	OpenDoorBoxOverlap->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CloseDoorBoxOverlap->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AElectronicDoor::BeginPlay()
{
	Super::BeginPlay();
	if (HaveElectricity()) BindCallbacks();
}

void AElectronicDoor::BindCallbacks()
{
	// Update electricity indicator material
	if (ElectricityIndicatorMesh) {
		UMaterialInstanceDynamic* MI = ElectricityIndicatorMesh->CreateDynamicMaterialInstance(0);
		if (MI) {
			if (HaveElectricity()) {
				MI->SetScalarParameterValue("Red", 0);
				MI->SetScalarParameterValue("Green", 1);
			}
			else {
				MI->SetScalarParameterValue("Red", 1);
				MI->SetScalarParameterValue("Green", 0);
			}
		}
	}

	if (OpenDoorBoxOverlap && CloseDoorBoxOverlap)
	{
		OpenDoorBoxOverlap->OnComponentBeginOverlap.AddDynamic(this, &AElectronicDoor::OnOpenDoorBoxOverlap);
		CloseDoorBoxOverlap->OnComponentEndOverlap.AddDynamic(this, &AElectronicDoor::OnCloseDoorBoxOverlap);
	}
}

void AElectronicDoor::OnElectricityTurnedOn()
{
	BindCallbacks();
}

void AElectronicDoor::OnOpenDoorBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;
	
	if (IsLocked())
	{
		if (!TryUnlockDoor(OtherActor))
			return;
	}
	OpenDoor();
}

void AElectronicDoor::OnCloseDoorBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->ActorHasTag("Player")) return;
	if (IsLocked()) return;
	CloseDoor();
}

void AElectronicDoor::OpenDoor()
{
	bOpening = true;
	OnOpenDoor.Broadcast();
}

void AElectronicDoor::CloseDoor()
{
	bClosing = true;
	OnCloseDoor.Broadcast();
}

bool AElectronicDoor::TryUnlockDoor(AActor* Caller)
{
	return false;
}

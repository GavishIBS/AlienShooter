// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ElectronicActor.h"
#include "ElectronicDoor.generated.h"

class UBoxComponent;

UENUM()
enum class EMovementAxis
{
	X, Y, Z
};

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AElectronicDoor : public AElectronicActor
{
	GENERATED_BODY()
	
public:
	AElectronicDoor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void BindCallbacks();
	virtual void OnElectricityTurnedOn();


	UFUNCTION()
	void OnOpenDoorBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnCloseDoorBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual bool TryUnlockDoor(AActor* Caller);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> OpenDoorBoxOverlap;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> CloseDoorBoxOverlap;

private:
	void OpenDoor();
	void CloseDoor();

private:
	bool bOpening = false;
	bool bClosing = false;

	//UPROPERTY(BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FVector InitialDoorLocation;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FVector TargetLocationOffset = FVector(0.f, 0.f, -200.f);
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	float DoorSpeed = 6.f;

public:
	FORCEINLINE virtual bool IsLocked() const { return false; }
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectricitySwitchboard.generated.h"

class ALight;
class UBoxComponent;
class AElectronicActor;

UCLASS()
class ALIENSHOOTER_API AElectricitySwitchboard : public AActor
{
	GENERATED_BODY()
	
public:	
	AElectricitySwitchboard();

protected:
	virtual void BeginPlay() override;

	void BindCallbacks();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> OverlapBox;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Controlled")
	TArray<AElectronicActor*> ControlledActors;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSwitchboard.generated.h"

class ALight;
class UBoxComponent;

UCLASS()
class ALIENSHOOTER_API ALightSwitchboard : public AActor
{
	GENERATED_BODY()
	
public:	
	ALightSwitchboard();

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
	void ToggleVisibility(bool bVisibility);

	UPROPERTY(EditAnywhere, Category = "Lights")
	TArray<ALight*> ControlledLightInstances;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshReplacer.generated.h"

class AStaticMeshActor;

UCLASS()
class ALIENSHOOTER_API AMeshReplacer : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeshReplacer();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(CallInEditor)
	void Replace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	TArray<AStaticMeshActor*> MeshesToReplace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	TArray<TSubclassOf<AActor>> ActorToBePlacedClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	FVector ScalingFactor = FVector(1, 1, 1);
};

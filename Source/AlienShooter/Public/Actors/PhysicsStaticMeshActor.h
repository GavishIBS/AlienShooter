// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/PhysicsActorInterface.h"
#include "PhysicsStaticMeshActor.generated.h"

UCLASS()
class ALIENSHOOTER_API APhysicsStaticMeshActor : public AActor, public IPhysicsActorInterface
{
	GENERATED_BODY()
	
public:	
	APhysicsStaticMeshActor();
	virtual UPrimitiveComponent* GetPhysicsComponent() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UStaticMeshComponent> Mesh;
};

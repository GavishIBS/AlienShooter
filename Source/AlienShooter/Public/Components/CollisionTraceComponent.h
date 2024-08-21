// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionTraceComponent.generated.h"

UENUM()
enum ETraceShape : uint8
{
	ETS_Line = 0,
	ETS_Sphere = 1,
	ETS_Box = 2
};

USTRUCT(Blueprintable)
struct FTraceSet
{
	GENERATED_USTRUCT_BODY()

	FTraceSet() {}
	
	UPROPERTY(EditDefaultsOnly, Category=Properties)
	FName TraceStartSocketName = FName("TraceStart");
	
	UPROPERTY(EditDefaultsOnly, Category=Properties)
	FName TraceEndSocketName = FName("TraceEnd");
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UCollisionTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCollisionTraceComponent();
	void InitComponent(USkeletalMeshComponent* InMesh);

	FHitResult DoTraceSingle(const TArray<AActor*>& ActorsToIgnore, const int Index);
	TArray<FHitResult> DoTraceMulti(const TArray<AActor*>& ActorsToIgnore, const int Index);
	
protected:
	virtual void BeginPlay() override;

	TObjectPtr<USkeletalMeshComponent> OwnerMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category=Properties)
	TEnumAsByte<ETraceShape> TraceShape = ETS_Sphere;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	TArray<FTraceSet> TraceSets = TArray<FTraceSet>();

	FTraceSet TraceSet;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	float TraceRadius = 10.f;
};

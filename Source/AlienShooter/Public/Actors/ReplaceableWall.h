// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplaceableWall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMeshReplaced);

UCLASS()
class ALIENSHOOTER_API AReplaceableWall : public AActor
{
	GENERATED_BODY()
	
public:	
	AReplaceableWall();

	UFUNCTION(BlueprintCallable)
	void ReplaceMesh();

	virtual void OnMeshChanged();

	UPROPERTY(BlueprintAssignable)
	FOnMeshReplaced OnMeshReplaced;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category=Components)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category=Data)
	TObjectPtr<UStaticMesh> ReservedMesh;

	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bReverseable = false;
};

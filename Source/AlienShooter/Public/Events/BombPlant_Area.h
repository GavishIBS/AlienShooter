// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Events/EventArea.h"
#include "BombPlant_Area.generated.h"

class ADynamite;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ABombPlant_Area : public AEventArea
{
	GENERATED_BODY()
	
public:
	ABombPlant_Area();

protected:
	virtual void BeginPlay() override;
	virtual void BindCallbacks() override;

	UFUNCTION()
	void TryPlantBomb(AActor* OtherActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UStaticMeshComponent> BombPlatformMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	USceneComponent* BombPlantPoint;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADynamite> Bomb_Class;
};

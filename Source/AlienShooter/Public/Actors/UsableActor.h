// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/InventoryPickup.h"
#include "../Interfaces/UsableInterface.h"
#include "UsableActor.generated.h"

UCLASS()
class ALIENSHOOTER_API AUsableActor : public AInventoryPickup, public IUsableInterface
{
	GENERATED_BODY()
	
public:	
	AUsableActor();
	virtual AUsableActor* UseItem(ACharacter* User) override;

protected:
	virtual void BeginPlay() override;
	virtual bool CanUse(ACharacter* User, FHitResult& HitResult);

	// Properties
	UPROPERTY(EditAnywhere, Category = "Properties")
	float UseRange = 150.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TObjectPtr<UAnimMontage> ItemUseAnimMontage;

public:
	FORCEINLINE UAnimMontage* GetItemUseAnimMontage() { return ItemUseAnimMontage; };
};

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
	virtual void UseItem(ACharacter* User) override;

protected:
	virtual void BeginPlay() override;
	virtual bool CanUse(ACharacter* User);
};

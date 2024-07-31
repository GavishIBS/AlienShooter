// // Copyright (c) 2024 Gavish Bansal// // All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

class AAIControllerBase;

/**
 * 
 */
class ALIENSHOOTER_API IAIInterface
{
	GENERATED_BODY()

public:
	bool UpdateChaseTarget(AAIControllerBase* AIController);
	void ChaseTarget(AAIControllerBase* AIController);
	
	bool IsInAttackRange();
};

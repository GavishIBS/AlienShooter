// // Copyright (c) 2024 Gavish Bansal// // All rights reserved.


#include "Interfaces/AIInterface.h"
#include "AI/Controller/AIControllerBase.h"
#include "AITypes.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

bool IAIInterface::UpdateChaseTarget(AAIControllerBase* AIController)
{
	if (!AIController) return false;

	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(AIController, 0))
	{
		AIController->SetTargetCharacter(PlayerCharacter);
		return true;
	}
	return false;
}

void IAIInterface::ChaseTarget(AAIControllerBase* AIController)
{
	if (!AIController || !AIController->GetTargetCharacter()) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor((AActor*)AIController->GetTargetCharacter());
	MoveRequest.SetAcceptanceRadius(AIController->GetAIProperties().AcceptanceRadius);
	AIController->MoveTo(MoveRequest);
}

bool IAIInterface::IsInAttackRange()
{
	return false;
}

void IAIInterface::StartAttackTrace()
{
}

void IAIInterface::EndAttackTrace()
{
}

void IAIInterface::AttackTrace()
{
}

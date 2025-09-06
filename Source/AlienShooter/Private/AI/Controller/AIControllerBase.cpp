// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/Controller/AIControllerBase.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AAIControllerBase::AAIControllerBase()
{
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

bool AAIControllerBase::CanReachActor(AActor* TargetActor)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn || !TargetActor)
    {
        return false;
    }

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys)
    {
        return false;
    }

    // Use the pathfinding helper
    UNavigationPath* NavPath = NavSys->FindPathToActorSynchronously(
        GetWorld(),
        ControlledPawn->GetActorLocation(),
        TargetActor
    );

    if (NavPath && NavPath->IsValid() && !NavPath->IsPartial())
    {
        return true; // Path is valid and complete
    }

    return false; // No valid path
}



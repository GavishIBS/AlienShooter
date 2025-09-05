// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ELifeStatus : uint8
{
	ELS_None = 0, ELS_Alive = 1, ELS_Dead = 2
};

UENUM(BlueprintType)
enum class EPawnActionState : uint8
{
	EPAS_None = 0 UMETA(DisplayName="None"),
	EPAS_Idle = 1 UMETA(DisplayName = "Idle"),
	EPAS_Attacking = 2 UMETA(DisplayName = "Attacking"),
	EPAS_Attacking_Melee = 3 UMETA(DisplayName = "Attacking_Melee"),
	EPAS_Reloading = 4 UMETA(DisplayName = "Reloading"),
	EPAS_AloneTalking = 5 UMETA(DisplayName = "AloneTalking"), // using radio phone or self dialogue
	EPAS_SelectingItem = 6 UMETA(DisplayName = "SelectingItem"),
	EPAS_UsingItem = 7 UMETA(DisplayName = "UsingItem"),
	EPAS_InPersonTalking = 8 UMETA(DisplayName = "InPersonTalking"),  // either in conversation
	EPAS_Max = 99
};

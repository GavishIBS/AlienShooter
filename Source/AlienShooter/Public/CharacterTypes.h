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
	EPAS_None = 0,
	EPAS_Idle = 1,
	EPAS_Attacking = 2,
	EPAS_Attacking_Melee = 3,
	EPAS_SelectingItem = 4,
	EPAS_UsingItem = 5,
	EPAS_Max = 99
};

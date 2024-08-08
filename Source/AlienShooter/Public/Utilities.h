#pragma once

#include "CoreMinimal.h"

static float DamageToForceConversion(const float& Damage)
{
	return Damage * Damage * 100000.f;
}

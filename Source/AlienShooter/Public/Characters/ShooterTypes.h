#pragma once

#include "CoreMinimal.h"
#include "ShooterTypes.generated.h"

class AShooterCharacter;

USTRUCT(BlueprintType)
struct FShooterProperties
{
	GENERATED_USTRUCT_BODY()

public:
	friend AShooterCharacter;
};

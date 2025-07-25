#pragma once

#include "CoreMinimal.h"
#include "UI_Types.generated.h"

class UTexture2D;

UENUM(BlueprintType)
enum class EKeyGroup : uint8
{
	WASD, L, LEFT_MOUSE_BUTTON, LEFT_ALT
};

USTRUCT(BlueprintType)
struct FKeySmashProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties)
	EKeyGroup KeyGroup;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties)
	FString KeyActionDescription = FString();  // Describes the action performed by the respective key

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties)
	TObjectPtr<UTexture2D> KeyGroup_Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties)
	float BlinkFrequeny = 1;  // 0 for no blink
};

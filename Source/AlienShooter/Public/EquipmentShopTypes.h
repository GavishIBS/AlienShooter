#pragma once

#include "CoreMinimal.h"
#include "EquipmentShopTypes.generated.h"

class AWeaponBase;
class AInventoryPickup;

USTRUCT(BlueprintType)
struct FEquipmentDataToSell
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	TArray<TSubclassOf<AInventoryPickup>> InventoryItemsToSell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	TArray<TSubclassOf<AWeaponBase>> WeaponsToSell;
};

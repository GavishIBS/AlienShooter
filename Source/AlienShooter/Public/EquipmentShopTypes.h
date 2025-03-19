#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EquipmentShopTypes.generated.h"

class AWeaponBase;
class AInventoryPickup;

USTRUCT(BlueprintType)
struct FEquipmentDataToSell
{
	GENERATED_BODY()

public:
	//UDataTable* ItemsDataTable;
	// FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	TArray<FDataTableRowHandle> InventoryItemsToSell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	TArray<FDataTableRowHandle> WeaponsToSell;
};

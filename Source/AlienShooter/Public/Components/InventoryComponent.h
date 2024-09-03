// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utilities.h"
#include "InventoryComponent.generated.h"

class ABaseCharacter;
class AInventoryPickup;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	friend ABaseCharacter;
	UInventoryComponent();

	bool AddItemToInventory(const FItemData ItemData);
	bool RemoveItemFromInventory(const TSubclassOf<AInventoryPickup> ItemClass);
	int32 HaveEmpltySlot();
	int HaveItem(const TSubclassOf<AInventoryPickup> ItemClass);
	void ClearSlot(const int32 Index);

	// DEBUG
	UFUNCTION(BlueprintCallable)
	void PrintContent();
	// DEBUG

protected:
	virtual void BeginPlay() override;
	void InitInventory();

	UPROPERTY(EditAnywhere, Category = "Properties")
	int32 InventorySize = 4;

private:
	TArray<FSlotData> Content;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utilities.h"
#include "InventoryComponent.generated.h"

class ABaseCharacter;
class AShooterCharacter;
class AInventoryPickup;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSelected, AActor*, SelectedItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	friend ABaseCharacter;
	friend AShooterCharacter;
	UInventoryComponent();

	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category = Delegates)
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category = Delegates)
	FOnItemSelected OnItemSelected;

public:
	bool AddItemToInventory(const FItemData ItemData);
	bool RemoveItemFromInventory(const TSubclassOf<AInventoryPickup> ItemClass, bool bShouldDestroy = false);
	bool RemoveItemFromInventory(const int32 Index, bool bShouldDestroy = false);
	int32 HaveEmpltySlot();
	int HaveItem(const TSubclassOf<AInventoryPickup> ItemClass);
	void ClearSlot(const int32 Index);

	bool SelectItem(const int Index);
	bool UnselectItem();

	// DEBUG
	UFUNCTION(BlueprintCallable)
	void PrintContent();
	// DEBUG

protected:
	virtual void BeginPlay() override;
	void InitInventory();

	void UseButtonPressed();

	TObjectPtr<ACharacter> OwnerAsCharacter;

protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	int32 InventorySize = 4;

private:
	void DestroyCurrentItem(); // Destorys item currently in hand

private:
	TArray<FSlotData> Content;

	AInventoryPickup* ItemInHand = nullptr;

	int32 SelectedItemIndex = -1;  // -1 => Unselected
};

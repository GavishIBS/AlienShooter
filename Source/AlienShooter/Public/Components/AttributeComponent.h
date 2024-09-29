// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorTypes.h"
#include "AttributeComponent.generated.h"

class AArmor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	void TakeArmor(AArmor* InArmor);

protected:
	virtual void BeginPlay() override;

	/* Attributes begin */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	float MaxHealth = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	float Health = 100.f;
	/* Attributes end */

	// Armor
	AArmor* ArmorEquipped = nullptr;

public:
	FORCEINLINE float GetMaxHealth() const {return  MaxHealth;}
	FORCEINLINE float GetHealth() const {return Health;}

	FORCEINLINE void SetMaxHealth(const float Value) {MaxHealth = Value;}
	FORCEINLINE void SetHealth(const float Value) {Health = Value;}

	FORCEINLINE void AddHealth(const float Value) { Health += Value; }

	FORCEINLINE bool CanTakeArmor(const EArmorColorCode ArmorColorCode);

	// TODO: Clamp Health
};

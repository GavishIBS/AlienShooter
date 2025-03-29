// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorTypes.h"
#include "AttributeComponent.generated.h"

class AArmor;

USTRUCT(BlueprintType)
struct FDeathAssetsData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Data")
	UAnimMontage* DeathAnimMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Data")
	UParticleSystem* DeathParticleSystem = nullptr;
	UPROPERTY(EditAnywhere, Category = "Data")
	FName DeathParticleSystemSpawnSocketName = FName("DeathEmitterSocket");
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENSHOOTER_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	void TakeDamage(const float InDamage);

	void TakeArmor(AArmor* InArmor);
	void BreakArmor();

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

	// Death
	UPROPERTY(EditDefaultsOnly, Category = "Death | Data")
	FDeathAssetsData DeathAssetsData;

public:
	FORCEINLINE float GetMaxHealth() const {return  MaxHealth;}
	FORCEINLINE float GetHealth() const {return Health;}

	FORCEINLINE void SetMaxHealth(const float Value) {MaxHealth = Value;}
	FORCEINLINE void SetHealth(const float Value)
	{
		Health = FMath::Clamp(Value, 0.f, MaxHealth);
	}

	FORCEINLINE void AddHealth(const float Value)
	{
		Health = FMath::Clamp(Health + Value, 0.f, MaxHealth);
	}

	FORCEINLINE bool CanTakeArmor(const EArmorColorCode ArmorColorCode);

	FORCEINLINE UAnimMontage* GetDeathAnimMontage() const { return DeathAssetsData.DeathAnimMontage; }
	FORCEINLINE FName GetRandomDeathAnimMontageSectionName() const { return FName("Default"); }
	FORCEINLINE UParticleSystem* GetDeathEmitter() const { return DeathAssetsData.DeathParticleSystem; }
	FORCEINLINE FName GetDeathEmitterSpawnSocketName() const { return DeathAssetsData.DeathParticleSystemSpawnSocketName; }
};

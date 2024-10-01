// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/Pickup.h"
#include "ArmorTypes.h"
#include "Armor.generated.h"

class UAttributeComponent;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AArmor : public APickup
{
	GENERATED_BODY()

public:
	friend UAttributeComponent;
	AArmor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void TakeDamage(const float InDamage);

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

	void Break();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	FArmorProperties ArmorProperties = FArmorProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	EArmorColorCode ArmorColorCode = EArmorColorCode::ECC_None;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TObjectPtr<UParticleSystem> DestructionParticleSystem;

	// TODO: Make character wear armor
	//TMap<UStaticMeshComponent, FName> -> TMap<Mesh, SocketName>

public:
	FORCEINLINE EArmorColorCode GetArmorColorCode() const { return ArmorColorCode; }
	FORCEINLINE bool IsDamaged() const
	{
		bool ReturnValue = ArmorProperties.CurrentArmorHealth >= ArmorProperties.MaxArmorHealth ? false : true;
		return ReturnValue;
	}
	
	FORCEINLINE FArmorProperties GetArmorProperties() const { return ArmorProperties; }
	FORCEINLINE float GetArmorHealth() const { return ArmorProperties.CurrentArmorHealth; }
	FORCEINLINE float GetArmorDamageAbsorption() const { return ArmorProperties.ArmorDamageAbsorption; }
	FORCEINLINE FName GetAttachmentSocketName() const { return ArmorProperties.AttachmentSocketName; }
};

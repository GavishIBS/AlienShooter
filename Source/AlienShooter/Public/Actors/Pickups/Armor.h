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

	void AbsorbDamage(const float InDamage);

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

	void Break();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	FArmorProperties ArmorProperties = FArmorProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	EArmorColorCode ArmorColorCode = EArmorColorCode::ECC_None;

	UPROPERTY(EditAnywhere, Category = "FX")
	TObjectPtr<UParticleSystem> DestructionParticleSystem;

	UPROPERTY(EditAnywhere, Category = "ArmorPieces")
	TArray<FName> ArmorPiecesAttachmentSocketNames;

	UPROPERTY(EditAnywhere, Category = "ArmorPieces")
	TArray<UStaticMeshComponent*> ArmorPiecesMeshComponents;

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
	FORCEINLINE TArray<FName> GetArmorPiecesAttachmentSocketNames() const { return ArmorPiecesAttachmentSocketNames; }
	
	FORCEINLINE UStaticMeshComponent* GetArmorPieceMeshComponent_AtIndex(const int Index) const
	{
		if (ArmorPiecesMeshComponents.Num() <= Index) return nullptr;
		return ArmorPiecesMeshComponents[Index];
	}
};

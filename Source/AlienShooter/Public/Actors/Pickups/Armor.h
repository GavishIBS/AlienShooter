// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/Pickup.h"
#include "ArmorTypes.h"
#include "Armor.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AArmor : public APickup
{
	GENERATED_BODY()
	
public:
	AArmor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	FArmorProperties ArmorProperties = FArmorProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	EArmorColorCode ArmorColorCode = EArmorColorCode::ECC_None;

	// TODO: Make character wear armor
	//TMap<UStaticMeshComponent, FName> -> TMap<Mesh, SocketName>

public:
	FORCEINLINE EArmorColorCode GetArmorColorCode() const { return ArmorColorCode; }
	FORCEINLINE bool IsDamaged() const
	{
		bool ReturnValue = ArmorProperties.CurrentArmorHealth >= ArmorProperties.MaxArmorHealth ? false : true;
		return ReturnValue;
	}
};

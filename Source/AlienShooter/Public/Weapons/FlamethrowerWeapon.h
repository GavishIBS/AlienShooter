// // Copyright (c) 2024 Gavish Bansal// // All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TracerWeapon.h"
#include "FlamethrowerWeapon.generated.h"

class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API AFlamethrowerWeapon : public ATracerWeapon
{
	GENERATED_BODY()
	
public:
	AFlamethrowerWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Fire() override;


	// Properties
	UPROPERTY(EditAnywhere, Category = "Properties")
	float TraceRange = 350.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float TraceRadiusAdditive = 32.5f;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (ClampMin = "1"))
	int32 NumberOfSphereTraceDivisions = 8;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (ClampMin = "0"))
	int32 AffectVelocityAfterTraces = 2;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (ClampMin = "0"))
	float VelocityAffectingScaling = 12.5f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bCheckObstaclesBetween = true;

	// Effects
	UPROPERTY(EditDefaultsOnly, Category = "Effects / FX")
	UNiagaraComponent* FlameFXComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects / FX")
	float FlameFXDeactivateTime = 0.35f;

private:
	FTimerHandle FlameFXTimer;

	void DisableFlameFX();
	void EnableFlameFX();
};

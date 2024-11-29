// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeaponBase.generated.h"

UCLASS()
class ALIENSHOOTER_API AMeleeWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeleeWeaponBase();

	void StartAttackTrace();
	void StopAttackTrace();

protected:
	virtual void BeginPlay() override;

	virtual void PerformAttackTrace();

	UPROPERTY(VisibleAnywhere, Category=Components)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FName AttachmentSocketName;

	UPROPERTY(EditAnywhere, Category = "Properties")
	UAnimMontage* AttackAnimMontage;
	
	//UPROPERTY(EditAnywhere, Category = "Properties")
	//UAnimMontage* PowerAttackAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FName AttackTraceStart_SocketName = FName("AttackTraceStartSocket");

	UPROPERTY(EditAnywhere, Category = "Properties")
	FName AttackTraceEnd_SocketName = FName("AttackTraceEndSocket");

	UPROPERTY(EditAnywhere, Category = "Properties")
	float TraceRadius = 5.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float AttackTraceRate = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float MaxDamagePerAttack = 180.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamagePerTrace = 37.5f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamagePerTrace_Randomization = 10.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	bool bFreezeTick_InAttack = true;

private:
	FTimerHandle AttackTraceTimerHandle;
	float DamageDealt_ThisAttack = 0.f;

public:
	FORCEINLINE FName GetAttachmentSocketName() const { return AttachmentSocketName; }
	FORCEINLINE UAnimMontage* GetAttackMontage(const bool bPowerAttack) { return bPowerAttack ? nullptr : AttackAnimMontage; }
	FORCEINLINE bool ShouldFreezeTickInAttack() const { return bFreezeTick_InAttack; }
};

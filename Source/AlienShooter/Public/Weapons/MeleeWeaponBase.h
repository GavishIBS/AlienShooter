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

private:
	FTimerHandle AttackTraceTimerHandle;

public:
	FORCEINLINE FName GetAttachmentSocketName() const { return AttachmentSocketName; }
	FORCEINLINE UAnimMontage* GetAttackMontage(const bool bPowerAttack) { return bPowerAttack ? nullptr : AttackAnimMontage; }
};

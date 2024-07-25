// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USphereComponent;

UCLASS()
class ALIENSHOOTER_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	bool TakeWeapon(const ACharacter* InstigatorCharacter);

protected:
	void Init_Callbacks();
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<USphereComponent> OverlapSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<UStaticMeshComponent> WeaponStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FWeaponProperties WeaponProperties = FWeaponProperties();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data)
	FWeaponData WeaponData = FWeaponData();

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshAsSkeletalMesh() const {return WeaponSkeletalMesh;}
	FORCEINLINE UStaticMeshComponent* GetWeaponMeshAsStaticMesh() const {return WeaponStaticMesh;}
	FORCEINLINE FWeaponProperties GetWeaponProperties() const {return WeaponProperties;}
	FORCEINLINE TSubclassOf<AWeaponBase> GetWeaponClass() const {return WeaponData.WeaponClass;}
};

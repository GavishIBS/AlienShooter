// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UArrowComponent;
class AShooterCharacter;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponPicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFirePressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFired);

UCLASS()
class ALIENSHOOTER_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category=Delegates)
	FOnWeaponPicked OnWeaponPicked;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category=Delegates)
	FOnWeaponSelected OnWeaponSelected;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category=Delegates)
	FOnWeaponFirePressed OnWeaponFirePressed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category=Delegates)
	FOnWeaponFired OnWeaponFired;
	
	//bool TakeWeapon(const ACharacter* InstigatorCharacter);

protected:
	virtual void BeginPlay() override;
	void Init_Callbacks();
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponPickedCallback();

	UFUNCTION()
	void OnWeaponSelectedCallback();

	UFUNCTION()
	void OnWeaponFirePressedCallback();

	bool CanFire() const;
	virtual void Fire();
	virtual void WeaponFiredFailed();
	void SpawnBulletShell() const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Ownership)
	TObjectPtr<AShooterCharacter> OwnerAsShooterCharacter = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<USphereComponent> OverlapSphere;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<UStaticMeshComponent> WeaponStaticMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<USceneComponent> WeaponMuzzle_S;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<UArrowComponent> MuzzleDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FWeaponProperties WeaponProperties = FWeaponProperties();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data)
	FWeaponData WeaponData = FWeaponData();

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshAsSkeletalMesh() const {return WeaponSkeletalMesh;}
	FORCEINLINE UStaticMeshComponent* GetWeaponMeshAsStaticMesh() const {return WeaponStaticMesh;}
	FORCEINLINE const FWeaponProperties* GetWeaponProperties() const {return &WeaponProperties;}
	FORCEINLINE TSubclassOf<AWeaponBase> GetWeaponClass() const {return WeaponData.WeaponClass;}
	FORCEINLINE FTransform GetWeaponMuzzleTransform() const;
	FORCEINLINE int GetTotalAmmoAvailable() const {return WeaponProperties.GetTotalAmmoAvailable();}
	FORCEINLINE bool ShouldEnableRightHandIK() const {return WeaponProperties.bShouldEnableRightHandIK;}
	FORCEINLINE bool ShouldEnableFabrik() const {return WeaponProperties.bShouldEnableFabrik;}
	FORCEINLINE FTransform GetLeftHandPlacementTransform() const;
	
	FORCEINLINE void SetTotalAmmoAvailable(const int& Value) {WeaponProperties.SetTotalAmmoAvailable(Value);}
};

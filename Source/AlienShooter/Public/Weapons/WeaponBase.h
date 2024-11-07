// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "SpecialWeaponTypes.h"
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

	// TODO: Remove from public section and use delegates
	void UpdateLaser(const FVector& EndLocation, const bool& bHittableAhead);

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

	// Laser
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> LaserMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FWeaponProperties WeaponProperties = FWeaponProperties();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FWeaponAnimationData WeaponAnimationData = FWeaponAnimationData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FWeaponAnimationData_Pawn WeaponAnimationData_Pawn = FWeaponAnimationData_Pawn();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Properties)
	FLaserProperties LaserProperties = FLaserProperties();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data)
	FWeaponData WeaponData = FWeaponData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Properties)
	FWeaponFXData WeaponFXData = FWeaponFXData();

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshAsSkeletalMesh() const {return WeaponSkeletalMesh;}
	FORCEINLINE UStaticMeshComponent* GetWeaponMeshAsStaticMesh() const {return WeaponStaticMesh;}
	FORCEINLINE const FWeaponProperties* GetWeaponProperties() const {return &WeaponProperties;}
	FORCEINLINE const FWeaponAnimationData* GetWeaponAnimationData() const {return &WeaponAnimationData;}
	FORCEINLINE const FWeaponAnimationData_Pawn* GetWeaponAnimationData_Pawn() const {return &WeaponAnimationData_Pawn;}
	FORCEINLINE TSubclassOf<AWeaponBase> GetWeaponClass() const {return WeaponData.WeaponClass;}
	FORCEINLINE int GetTotalAmmoAvailable() const {return WeaponProperties.GetTotalAmmoAvailable();}
	FORCEINLINE bool ShouldEnableRightHandIK() const {return WeaponProperties.bShouldEnableRightHandIK;}
	FORCEINLINE bool ShouldEnableFabrik() const {return WeaponProperties.bShouldEnableFabrik;}
	FORCEINLINE bool HaveAutomaticFire() const { return WeaponProperties.bAutoFire; }
	FORCEINLINE float GetAutomaticFiringRate() const { return WeaponProperties.AutoFire_Rate; }
	FORCEINLINE int GetMaxAmmoCapacity() const { return WeaponProperties.MaxAmmoCapacity; }
	FORCEINLINE bool EnableLaser() const { return LaserProperties.bLaser; }
	
	FTransform GetWeaponMuzzleTransform() const;
	FTransform GetLeftHandPlacementTransform() const;

	FORCEINLINE void SetTotalAmmoAvailable(const int& Value) {WeaponProperties.SetTotalAmmoAvailable(Value);}
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Weapons/CursorWeapon.h"
#include "GameFramework/Actor.h"
#include "Characters/ShooterCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// DEBUG
#include "DrawDebugHelpers.h"

ACursorWeapon::ACursorWeapon()
{
}

void ACursorWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ACursorWeapon::Fire()
{
	Super::Fire();

	if (!OwnerAsShooterCharacter || !OwnerAsShooterCharacter->GetPlayerController()) return;

	float BulletHealth = WeaponProperties.BulletHealth;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(OwnerAsShooterCharacter);

	FHitResult HitResult;
	OwnerAsShooterCharacter->GetPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (HitResult.bBlockingHit)
	{
		FVector StartD = HitResult.ImpactPoint;
		FVector EndD = HitResult.ImpactPoint + (HitResult.ImpactNormal * 100);
		DrawDebugLine(GetWorld(), StartD, EndD, FColor::Red, false, 7.f);

		// Find two orthogonal vectors to define the plane's basis
		FVector BasisX, BasisY;
		HitResult.ImpactNormal.FindBestAxisVectors(BasisX, BasisY);

		// Generate random offset relative to the impact normal plane
		const float R_X = FMath::RandRange(-WeaponProperties.HitLocation_RandomizationAdditiveRange.X, WeaponProperties.HitLocation_RandomizationAdditiveRange.X);
		const float R_Y = FMath::RandRange(-WeaponProperties.HitLocation_RandomizationAdditiveRange.Y, WeaponProperties.HitLocation_RandomizationAdditiveRange.Y);

		const FVector RandomPoint = HitResult.ImpactPoint + (BasisX * R_X) + (BasisY * R_Y);

		FHitResult HitResult_M;
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), RandomPoint, RandomPoint, 7.f, ETraceTypeQuery::TraceTypeQuery1, false,
			ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult_M, true, FLinearColor::Red, FLinearColor::Green);

		if (!HitResult_M.bBlockingHit) return;
		if (IHitInterface* HitInterface = Cast<IHitInterface>(HitResult_M.GetActor()))
		{
			BulletHealth -= HitInterface->GetHit(HitResult_M.ImpactPoint, WeaponProperties.Damage, WeaponProperties.Damage_RandomizationFactor, OwnerAsShooterCharacter);

			// DEBUG
			DrawDebugSphere(GetWorld(), HitResult_M.ImpactPoint, 5.f, 24, FColor::Orange, false, 4.f);
		}
		OnBulletHit(HitResult_M.GetActor(), HitResult_M.ImpactPoint);
		if (BulletHealth <= 0.f) {}
	}
}

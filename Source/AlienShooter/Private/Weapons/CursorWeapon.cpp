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
	//FVector BasisX, BasisY;
	if (HitResult.bBlockingHit)
	{
		// Check for blocking object inbetween BEGIN
		{
			FHitResult HitBetween;
			UKismetSystemLibrary::LineTraceSingle(this, OwnerAsShooterCharacter->GetActorLocation(), HitResult.ImpactPoint, ETraceTypeQuery::TraceTypeQuery1, false,
				{ this, OwnerAsShooterCharacter }, EDrawDebugTrace::None, HitBetween, true);
			if (HitBetween.bBlockingHit)
			{
				const float DistBtw = FVector::Dist(HitResult.ImpactPoint, HitBetween.ImpactPoint);
				if (DistBtw > DistanceToInBetweenHit)
				{
					HitResult.ImpactPoint = (FVector_NetQuantize)HitBetween.ImpactPoint;

					// Check the direction character us facing and based on that edit the ImpactNormal vector
					FVector Forward = OwnerAsShooterCharacter->GetMesh()->GetForwardVector();
					Forward.Z = 0.f;
					Forward.Normalize();
					FVector WorldX = FVector(1, 0, 0);
					FVector WorldY = FVector(0, 1, 0);
					float DotX = FVector::DotProduct(Forward, WorldX);
					float DotY = FVector::DotProduct(Forward, WorldY);
					if (FMath::Abs(DotX) > FMath::Abs(DotY))
					{
						HitResult.ImpactNormal = FVector(0, 1, 0);
					}
					else {
						HitResult.ImpactNormal = FVector(1, 0, 0);
					}
				}
			}
		}
		// Check for blocking object inbetween END

		FVector StartD = HitResult.ImpactPoint;
		FVector EndD = HitResult.ImpactPoint + (HitResult.ImpactNormal * 100);
		// DEBUG
		//DrawDebugLine(GetWorld(), StartD, EndD, FColor::Red, false, 7.f);

		// Find two orthogonal vectors to define the plane's basis // finds axis which makes a plane at an angle of 90 degrees from the normal vector
		FVector BasisX, BasisY;
		HitResult.ImpactNormal.FindBestAxisVectors(BasisX, BasisY);

		// Generate random offset relative to the impact normal plane
		const float R_X = FMath::RandRange(-WeaponProperties.HitLocation_RandomizationAdditiveRange.X, WeaponProperties.HitLocation_RandomizationAdditiveRange.X);
		const float R_Y = FMath::RandRange(-WeaponProperties.HitLocation_RandomizationAdditiveRange.Y, WeaponProperties.HitLocation_RandomizationAdditiveRange.Y);

		const FVector RandomPoint = HitResult.ImpactPoint + (BasisX * R_X) + (BasisY * R_Y);

		FHitResult HitResult_M;
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), RandomPoint, RandomPoint, 7.f, ETraceTypeQuery::TraceTypeQuery1, false,
			ActorsToIgnore, EDrawDebugTrace::None, HitResult_M, true, FLinearColor::Red, FLinearColor::Green);

		if (!HitResult_M.bBlockingHit) return;
		if (IHitInterface* HitInterface = Cast<IHitInterface>(HitResult_M.GetActor()))
		{
			BulletHealth -= HitInterface->GetHit(HitResult_M.ImpactPoint, WeaponProperties.Damage, WeaponProperties.Damage_RandomizationFactor, OwnerAsShooterCharacter, UDamageType::StaticClass());

			// DEBUG
			//DrawDebugSphere(GetWorld(), HitResult_M.ImpactPoint, 5.f, 24, FColor::Orange, false, 4.f);
		}
		OnBulletHit(HitResult_M.GetActor(), HitResult_M.ImpactPoint, HitResult_M.PhysMaterial.Get());
		if (BulletHealth <= 0.f) {}
	}
}

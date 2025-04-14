// // Copyright (c) 2024 Gavish Bansal// // All rights reserved.


#include "Weapons/FlamethrowerWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"

AFlamethrowerWeapon::AFlamethrowerWeapon()
{
}

void AFlamethrowerWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AFlamethrowerWeapon::Fire()
{
	Super::Fire();

	const FVector OwnerVelocity = GetOwner() ? GetOwner()->GetVelocity() : FVector(0, 0, 0);
	const FTransform MuzzleTransform = WeaponProperties.bIsSkeletalMesh ? WeaponSkeletalMesh->GetSocketTransform(WeaponProperties.WeaponMuzzleSocketName) :
		WeaponStaticMesh->GetSocketTransform(WeaponProperties.WeaponMuzzleSocketName);

	const float TraceRadius = (TraceRange / NumberOfSphereTraceDivisions) + TraceRadiusAdditive;
	for (int i = 0; i < NumberOfSphereTraceDivisions; i++)
	{
		FHitResult OutHit;
		FVector TraceLocation = MuzzleTransform.GetLocation() + ((WeaponMuzzle_S->GetForwardVector() * TraceRadius) * (i + 1));
		if (i >= AffectVelocityAfterTraces)
		{
			TraceLocation += OwnerVelocity.GetSafeNormal() * VelocityAffectingScaling * (i + 1);
		}
		UKismetSystemLibrary::SphereTraceSingle(this, TraceLocation, TraceLocation, TraceRadius, ETraceTypeQuery::TraceTypeQuery1, false, {this, GetOwner()},
			EDrawDebugTrace::ForDuration, OutHit, true);
		if (OutHit.bBlockingHit)
		{
			if (IHitInterface* HitInterface = Cast<IHitInterface>(OutHit.GetActor()))
			{
				WeaponProperties.BulletHealth -= HitInterface->GetHit(OutHit.ImpactPoint, WeaponProperties.Damage, WeaponProperties.Damage_RandomizationFactor, GetOwner(), UDamageType::StaticClass());
			}
			OnBulletHit(OutHit.GetActor(), OutHit.ImpactPoint);
			if (WeaponProperties.BulletHealth <= 0) break;  // break loop if bullet health is or bwlow 0
		}
	}
}

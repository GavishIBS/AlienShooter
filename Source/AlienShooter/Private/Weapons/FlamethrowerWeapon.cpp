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

	float TraceRadius = (TraceRange / NumberOfSphereTraceDivisions) + TraceRadiusAdditive;
	int32 L_NumberOfSphereTraceDivisions = NumberOfSphereTraceDivisions;

	// Check if obstacle ahead
	if (bCheckObstaclesBetween)
	{
		FHitResult LineHit;
		FVector Start = WeaponMuzzle_S->K2_GetComponentLocation();
		FVector End = Start + WeaponMuzzle_S->GetForwardVector() * TraceRange;
		UKismetSystemLibrary::LineTraceSingle(this, Start, End, ETraceTypeQuery::TraceTypeQuery1, false, { this, GetOwner() }, EDrawDebugTrace::ForDuration, LineHit, true);
		if (LineHit.bBlockingHit == true && LineHit.GetActor() != nullptr)
		{
			if (LineHit.GetActor()->ActorHasTag(FName("Insulator")) == true) // TODO: Use physics materials for this logic
			{
				float L_TraceRange = FVector::Dist(WeaponMuzzle_S->GetComponentLocation(), LineHit.ImpactPoint);
				//L_TraceRange -= TraceRadius;
				L_NumberOfSphereTraceDivisions = L_TraceRange / TraceRadius;
				if (L_NumberOfSphereTraceDivisions <= 0)
				{
					// Adjust radius and do a single trace if not enough distance
					TraceRadius = L_TraceRange / 2;
					L_NumberOfSphereTraceDivisions = 1;
				}
				
			}
		}
	}

	for (int i = 0; i < L_NumberOfSphereTraceDivisions; i++)
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

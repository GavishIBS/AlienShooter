// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/Pickups/Usables/StickyBomb.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

AStickyBomb::AStickyBomb()
{
}

void AStickyBomb::BeginPlay()
{
	Super::BeginPlay();
}

bool AStickyBomb::CanUse(ACharacter* User)
{
	return true;
}

void AStickyBomb::UseItem(ACharacter* User)
{
	if (!CanUse(User)) return;

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(User);
	FVector Start = User->GetActorLocation() + User->GetMesh()->GetForwardVector();
	FVector End = Start + (User->GetMesh()->GetForwardVector() * UseRange);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false,
		ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	if (HitResult.bBlockingHit)
	{
		

		SetActorLocation(HitResult.ImpactPoint);
		// TODO: Set rotation

		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
		AttachToActor(HitResult.GetActor(), AttachmentTransformRules);
	}
}

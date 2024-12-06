// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Actors/UsableActor.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

AUsableActor::AUsableActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUsableActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AUsableActor::CanUse(ACharacter* User, FHitResult& HitResult)
{
	if (UseRange < 0) return true;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(User);
	FVector Start = User->GetActorLocation() + User->GetMesh()->GetRightVector();
	FVector End = Start + (User->GetMesh()->GetRightVector() * UseRange);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false,
		ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	if (HitResult.bBlockingHit && HitResult.GetActor()) return true;

	return false;
}

bool AUsableActor::UseItem(ACharacter* User)
{
	return false;
}

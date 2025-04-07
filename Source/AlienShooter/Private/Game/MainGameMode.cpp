// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Game/MainGameMode.h"


#include "Characters/BaseCharacter.h"


#include "Kismet/GameplayStatics.h"

/*
void AMainGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (NewPlayer && NewPlayer->GetPawn())
	{
		//ABaseCharacter* BChar = Cast<ABaseCharacter>(NewPlayer->GetPawn());w
		BChar = Cast<ABaseCharacter>(NewPlayer->GetPawn());
		if (BChar) {
			BChar->OnDeath.AddDynamic(this, &AMainGameMode::TryRespawnPlayer);
		}
	}
}
*/

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	{
		//ABaseCharacter* BChar = Cast<ABaseCharacter>(NewPlayer->GetPawn());
		BChar = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (BChar) {
			BChar->OnDeath.AddDynamic(this, &AMainGameMode::TryRespawnPlayer);
		}
	}
}

void AMainGameMode::RespawnPlayer()
{
	BChar->Rebirth(LastCheckpointTransform);
	RespawnCount--;
}

void AMainGameMode::TryRespawnPlayer(AActor* Killer)
{
	if (CanRespawn())
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainGameMode::RespawnPlayer, 2.f, false, 2.f);
	}
	else {
		// Game over
	}
}

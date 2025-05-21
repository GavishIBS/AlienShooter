// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Game/MainGameMode.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Electricity/Actors/AlarmDirectionalLight_Electric.h"
#include "Components/AudioComponent.h"

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

	Setup_AlarmLights();
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

void AMainGameMode::EnableAlarmingSystem(const EAlarmingModeName& SwitchToModeName)
{
	if (CurrentAlarmingModeName != EAlarmingModeName::NONE) return;

	//CurrentAlarmingModeName = SwitchToModeName;
	//if (CurrentAlarmingModeName == EAlarmingModeName::NONE) CurrentAlarmingModeName = EAlarmingModeName::NORMAL;
	EAlarmingModeName LastModeName = CurrentAlarmingModeName;
	CurrentAlarmingModeName = SwitchToModeName == EAlarmingModeName::NONE ? EAlarmingModeName::NORMAL : SwitchToModeName;
	
	for (FAlarmingModeData& ModeData : AlarmingModes)
	{
		if (ModeData.AlarmingModeName == CurrentAlarmingModeName) {
			ModeData.OnAlarmingModeActivated.Broadcast(ModeData.AlarmingModeName);
			break;
		}
	}

	OnAlarmingModeChanged(CurrentAlarmingModeName, LastModeName);
}

void AMainGameMode::DisableAlarmingSystem()
{
	if (CurrentAlarmingModeName == EAlarmingModeName::NONE) return;

	for (FAlarmingModeData& ModeData : AlarmingModes)
	{
		if (ModeData.AlarmingModeName == CurrentAlarmingModeName) {
			ModeData.OnAlarmingModeDeactivated.Broadcast(ModeData.AlarmingModeName);
			break;
		}
	}

	EAlarmingModeName LastModeName = CurrentAlarmingModeName;
	CurrentAlarmingModeName = EAlarmingModeName::NONE;
	OnAlarmingModeChanged(CurrentAlarmingModeName, LastModeName);
}

void AMainGameMode::ChangeAlarmingMode(const EAlarmingModeName& SwitchToModeName)
{
	DisableAlarmingSystem();
	EnableAlarmingSystem(SwitchToModeName);
}

void AMainGameMode::OnAlarmingModeChanged(const EAlarmingModeName& NewModeName, const EAlarmingModeName& LastModeName)
{
	if (NewModeName == EAlarmingModeName::NONE) return;

	//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//FVector PlayerLocation;
	
	if (CurrentAlarmModeSound_AudioComponent) {
		CurrentAlarmModeSound_AudioComponent->Stop();
		CurrentAlarmModeSound_AudioComponent->DestroyComponent();
	}
	if (USoundBase* SoundToSpawn = GetAlarmingModeData_ByName(CurrentAlarmingModeName).SirensSoundEffect) {
		CurrentAlarmModeSound_AudioComponent = UGameplayStatics::SpawnSound2D(this, SoundToSpawn);
	}

	// disable last alarm light
	for (AAlarmDirectionalLight_Electric* AlarmLight : Alarm_DirectionalLights) {
		if (LastModeName == AlarmLight->GetLinkedAlarmingModeName()) {
			AlarmLight->SetLightVisibility(false);
		}
	}

	// enable new alarm light
	for (AAlarmDirectionalLight_Electric* AlarmLight : Alarm_DirectionalLights) {
		if (NewModeName == AlarmLight->GetLinkedAlarmingModeName()) {
			AlarmLight->SetLightVisibility(true);
		}
	}
}

FAlarmingModeData AMainGameMode::GetAlarmingModeData_ByName(const EAlarmingModeName& ModeName)
{
	FAlarmingModeData ReturnValue;
	for (FAlarmingModeData& ModeData : AlarmingModes) {
		if (ModeData.AlarmingModeName == ModeName) {
			ReturnValue = ModeData;
			break;
		}
	}
	return ReturnValue;
}

void AMainGameMode::Setup_AlarmLights()
{
	TArray<AActor*> AlarmLights_AsActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAlarmDirectionalLight_Electric::StaticClass(), AlarmLights_AsActors);

	for (AActor* AlarmLight_AsActor : AlarmLights_AsActors)
	{
		AAlarmDirectionalLight_Electric* AlarmLight = Cast<AAlarmDirectionalLight_Electric>(AlarmLight_AsActor);
		if (AlarmLight) {
			Alarm_DirectionalLights.AddUnique(AlarmLight);
			
			for (FAlarmingModeData& ModeData : AlarmingModes) {
				if (ModeData.AlarmingModeName == AlarmLight->GetLinkedAlarmingModeName()) {
					ModeData.DirectionalLight = AlarmLight;
				}
			}
		}
	}
}

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Game/MainGameMode.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Electricity/Actors/AlarmDirectionalLight_Electric.h"
#include "Components/AudioComponent.h"
#include "Electricity/Actors/Electric_Siren_SpotLight.h"
#include "Game/Subsystems/OptionsGameInstanceSubsystem.h"
#include "Electricity/Actors/ElectronicDoor.h"
#include "AI/AISpawnerBase.h"
#include "AI/SpawnStarter.h"

/*
void AMainGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (NewPlayer && NewPlayer->GetPawn())
	{
		//ABaseCharacter* BChar = Cast<ABaseCharacter>(NewPlayer->GetPawn());
		BChar = Cast<ABaseCharacter>(NewPlayer->GetPawn());
		if (BChar) {
			BChar->OnDeath.AddDynamic(this, &AMainGameMode::TryRespawnPlayer);
		}
	}
}
*/

void AMainGameMode::SkipSequence()
{
	EndSequence();
}

void AMainGameMode::OnSequencePlayed(float Duration)
{
	DisableControl();
	FTimerHandle ControlEnableTimerHandle;
	GetWorldTimerManager().SetTimer(ControlEnableTimerHandle, FTimerDelegate::CreateLambda([&] {
		EndSequence();
	}), Duration + ControlToggleOnSequence_Delay, false, Duration + ControlToggleOnSequence_Delay);
}

void AMainGameMode::EndSequence()
{
	EnableControl();
}

void AMainGameMode::DisableControl()
{
	if (BChar) {
		BChar->SetActorTickEnabled(false);
		BChar->DisableInput(Cast<APlayerController>(BChar->GetController()));
	} else {/* TODO: disable through controller directly */}
}

void AMainGameMode::EnableControl()
{
	if (BChar) {
		BChar->SetActorTickEnabled(true);
		BChar->EnableInput(Cast<APlayerController>(BChar->GetController()));
	} else {/* TODO: enable through controller directly */ }
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UOptionsGameInstanceSubsystem* OptionsGameInstSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UOptionsGameInstanceSubsystem>())
	{
		{   // Update audio controls
			if (GameSoundMix) {
				UGameplayStatics::PushSoundMixModifier(this, GameSoundMix);
				if (MusicSoundClass, EffectsSoundClass, DialoguesSoundClass) {
					UGameplayStatics::SetSoundMixClassOverride(this, GameSoundMix, MusicSoundClass, OptionsGameInstSubsystem->MusicVolume);
					UGameplayStatics::SetSoundMixClassOverride(this, GameSoundMix, EffectsSoundClass, OptionsGameInstSubsystem->EffectsVolume);
					UGameplayStatics::SetSoundMixClassOverride(this, GameSoundMix, DialoguesSoundClass, OptionsGameInstSubsystem->DialoguesVolume);
				}
			}
		}
		{   // Update post process controls
			AActor* PostProcessVolume_AsActor = UGameplayStatics::GetActorOfClass(this, APostProcessVolume::StaticClass());
			APostProcessVolume* PostProcessVolume = Cast<APostProcessVolume>(PostProcessVolume_AsActor);
			if (PostProcessVolume) {
				PostProcessVolume->Settings.AutoExposureBias += OptionsGameInstSubsystem->BrightnessAdditiveValue;
			}
		}
	}
	
	{
		//ABaseCharacter* BChar = Cast<ABaseCharacter>(NewPlayer->GetPawn());
		BChar = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (BChar) {
			BChar->OnDeath.AddDynamic(this, &AMainGameMode::TryRespawnPlayer);
		}
	}

	OnPlaySequence.AddDynamic(this, &AMainGameMode::OnSequencePlayed);

	Setup_AlarmLights();
	
	// Setup doors
	TArray<AActor*> Doors_AsActors;
	UGameplayStatics::GetAllActorsOfClass(this, AElectronicDoor::StaticClass(), Doors_AsActors);
	for (AActor* Door_AsActor : Doors_AsActors)
	{
		AElectronicDoor* Door = Cast<AElectronicDoor>(Door_AsActor);
		if (Door) {
			DoorsCache.AddUnique(Door);
		}
	}


	Cache_AISpawners();
	Cache_SpawnStarters();
}

void AMainGameMode::Cache_AISpawners()
{
	AISpawnersCache.Empty();

	TArray<AActor*> AISpawners_AsActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAISpawnerBase::StaticClass(), AISpawners_AsActors);
	for (AActor* AISpawner_AsActor : AISpawners_AsActors)
	{
		AAISpawnerBase* AISpawner = Cast<AAISpawnerBase>(AISpawner_AsActor);
		if (AISpawner) {
			AISpawnersCache.AddUnique(AISpawner);
		}
	}
}

void AMainGameMode::Cache_SpawnStarters()
{
	AISpawnStartersCache.Empty();

	TArray<AActor*> AISpawnStarters_AsActors;
	UGameplayStatics::GetAllActorsOfClass(this, ASpawnStarter::StaticClass(), AISpawnStarters_AsActors);
	for (AActor* AISpawnStarter_AsActor : AISpawnStarters_AsActors)
	{
		ASpawnStarter* AISpawnStarter = Cast<ASpawnStarter>(AISpawnStarter_AsActor);
		if (AISpawnStarter && AISpawnStarter->GetAISpawnerRef()) {
			AISpawnStartersCache.AddUnique(AISpawnStarter);
			AISpawnStarter->GetAISpawnerRef()->OnAIEnemySpawned.AddDynamic(this, &AMainGameMode::IncrementEnemyCount);
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
		UGameplayStatics::OpenLevel(this, FName("MainMenuLevel"));
	}
}

void AMainGameMode::EnableAlarmingSystem(const EAlarmingModeName& SwitchToModeName)
{
	//if (CurrentAlarmingModeName != EAlarmingModeName::NONE) return;

	EAlarmingModeName LastModeName = CurrentAlarmingModeName;
	CurrentAlarmingModeName = SwitchToModeName == EAlarmingModeName::NONE ? EAlarmingModeName::NORMAL : SwitchToModeName;
	
	for (FAlarmingModeData& ModeData : AlarmingModes)
	{
		if (ModeData.AlarmingModeName == CurrentAlarmingModeName) {
			ModeData.OnAlarmingModeActivated.Broadcast(ModeData.AlarmingModeName);
			
			FTimerHandle ResetAlarmingModeTimerHandle;
			TWeakObjectPtr<AMainGameMode> WeakThis(this);
			GetWorldTimerManager().SetTimer(ResetAlarmingModeTimerHandle, FTimerDelegate::CreateLambda([&] {
				if (!WeakThis.IsValid()) return;

				EnableAlarmingSystem(EAlarmingModeName::NORMAL);
			}), ModeData.ResetAfterSeconds, false);

			break;
		}
	}

	OnAlarmingModeChanged(CurrentAlarmingModeName, LastModeName);
}

void AMainGameMode::DisableAlarmingSystem()
{
	/*
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
	*/
	EnableAlarmingSystem();
}

void AMainGameMode::OnAlarmingModeChanged(const EAlarmingModeName& NewModeName, const EAlarmingModeName& LastModeName)
{
	if (NewModeName == EAlarmingModeName::NONE) return;

	/*
	if (CurrentAlarmModeSound_AudioComponent) {
		CurrentAlarmModeSound_AudioComponent->Stop();
		CurrentAlarmModeSound_AudioComponent->DestroyComponent();
	}
	*/
	for (UAudioComponent* AudioComponent : CurrentAlarmModeSound_AudioComponents)
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
	}
	CurrentAlarmModeSound_AudioComponents.Empty();
	for (USoundBase* SirensSoundEffect : GetAlarmingModeData_ByName(CurrentAlarmingModeName).SirensSoundEffects) {
		if (SirensSoundEffect) {
			CurrentAlarmModeSound_AudioComponents.Add(UGameplayStatics::SpawnSound2D(this, SirensSoundEffect));
		}
	}

	/*
	// disable last alarm light
	for (FAlarmingModeData& ModeData_LastLight : AlarmingModes)   // TODO: Optimize this code
	{
		if (ModeData_LastLight.AlarmingModeName == LastModeName) {
			if (ModeData_LastLight.DirectionalLight) {
				ModeData_LastLight.DirectionalLight->SetLightVisibility(false);
			}
		}

		// enable new alarm light
		for (FAlarmingModeData& ModeData_NiewLight : AlarmingModes)
		{
			if (ModeData_NiewLight.AlarmingModeName == NewModeName) {
				if (ModeData_NiewLight.DirectionalLight) { ModeData_NiewLight.DirectionalLight->SetLightVisibility(true); }
			}
		}

		// update sirens
		for (AElectric_Siren_SpotLight* Siren : SirensCache)
		{
			if (Siren) {
				Siren->TurnOnLightWithMode(NewModeName);
			}
		}
		
		// update doors
		for (FAlarmingModeData& AlarmingModeData : AlarmingModes)
		{
			if (AlarmingModeData.AlarmingModeName == NewModeName)
			{
				for (AElectronicDoor* Door : DoorsCache)
				{
					if (Door) {
						if (AlarmingModeData.bForceDoorsOpen) {
							Door->StartForceKeepOpen();
						}
						else {
							Door->EndForceKeepOpen();
						}
					}
				}
			}
		}

		// update cursor widget
		for (FAlarmingModeData& ModeData_Cursor : AlarmingModes)
		{
			if (ModeData_Cursor.AlarmingModeName == NewModeName) {
				if (ModeData_Cursor.CursorWidget) {
					GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->SetMouseCursorWidget(EMouseCursor::Default, ModeData_Cursor.CursorWidget);
				}
			}
		}

		OnAlarmingModeChangedEvent.Broadcast(NewModeName, LastModeName);
	}
	*/

	// disable last alarm light
	for (FAlarmingModeData& ModeData_LastLight : AlarmingModes)
	{
		if (ModeData_LastLight.AlarmingModeName == LastModeName) {
			if (ModeData_LastLight.DirectionalLight) {
				ModeData_LastLight.DirectionalLight->SetLightVisibility(false);
			}
		}
	}

	// enable new alarm light
	for (FAlarmingModeData& ModeData_NiewLight : AlarmingModes)
	{
		if (ModeData_NiewLight.AlarmingModeName == NewModeName) {
			if (ModeData_NiewLight.DirectionalLight) { ModeData_NiewLight.DirectionalLight->SetLightVisibility(true); }
		}
	}

	// update sirens
	for (AElectric_Siren_SpotLight* Siren : SirensCache)
	{
		if (Siren) {
			Siren->TurnOnLightWithMode(NewModeName);
		}
	}

	/*
	// update doors
	for (FAlarmingModeData& AlarmingModeData : AlarmingModes)
	{
		if (AlarmingModeData.AlarmingModeName == NewModeName)
		{
			for (AElectronicDoor* Door : DoorsCache)
			{
				if (Door) {
					if (AlarmingModeData.bForceDoorsOpen) {
						Door->StartForceKeepOpen();
					}
					else {
						Door->EndForceKeepOpen();
					}
				}
			}
		}
	}
	*/

	
	for (FAlarmingModeData& AlarmingModeData : AlarmingModes)
	{
		if (AlarmingModeData.AlarmingModeName == NewModeName)
		{
			{// update doors
				for (AElectronicDoor* Door : DoorsCache)
				{
					if (Door) {
						if (AlarmingModeData.bForceDoorsOpen) {
							Door->StartForceKeepOpen();
						}
						else {
							Door->EndForceKeepOpen();
						}
					}
				}
			}

			Cache_AISpawners();
			Cache_SpawnStarters();

			{// update AI spawn starters and AI spawners
				for (ASpawnStarter* SpawnStarter : AISpawnStartersCache)
				{
					if (SpawnStarter)
					{
						AlarmingModeData.bDisableAISpawnStarters ? SpawnStarter->DisableSpawnStarter() : SpawnStarter->EnableSpawnStarter();
					}
				}

				if (AlarmingModeData.AISpawnerTriggerChance > 0.f)
				{
					for (AAISpawnerBase* AISpawner : AISpawnersCache)
					{
						if (AlarmingModeData.AISpawnerTriggerChance >= (FMath::RandRange(0, 1) * 100))
						{
							if (AISpawner)
							{
								AISpawner->StartSpawningWrapper();
							}
						}
					}
				}
			}
		}
	}

	// update cursor widget
	for (FAlarmingModeData& ModeData_Cursor : AlarmingModes)
	{
		if (ModeData_Cursor.AlarmingModeName == NewModeName) {
			if (ModeData_Cursor.CursorWidget) {
				GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->SetMouseCursorWidget(EMouseCursor::Default, ModeData_Cursor.CursorWidget);
			}
		}
	}

	OnAlarmingModeChangedEvent.Broadcast(NewModeName, LastModeName);
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

void AMainGameMode::IncrementEnemyCount(AController* SpawnedEnemyControlled)
{
	if (SpawnedEnemyControlled) {
		SpawnedEnemyControlled->OnDestroyed.AddDynamic(this, &AMainGameMode::DecrementEnemyCount);
		CurrentEnemyNumber += 1;
	}
}

void AMainGameMode::DecrementEnemyCount(AActor* DestroyedActor)
{
	if (DestroyedActor) {
		CurrentEnemyNumber -= 1;
	}
}

void AMainGameMode::Setup_AlarmLights()
{
	TArray<AActor*> AlarmLights_AsActors;  // Directional lights
	UGameplayStatics::GetAllActorsOfClass(this, AAlarmDirectionalLight_Electric::StaticClass(), AlarmLights_AsActors);

	for (AActor* AlarmLight_AsActor : AlarmLights_AsActors)
	{
		AAlarmDirectionalLight_Electric* AlarmLight = Cast<AAlarmDirectionalLight_Electric>(AlarmLight_AsActor);
		if (AlarmLight) {
			for (FAlarmingModeData& ModeData : AlarmingModes) {
				if (ModeData.AlarmingModeName == AlarmLight->GetLinkedAlarmingModeName()) {
					ModeData.DirectionalLight = AlarmLight;
				}
			}
		}
	}

	TArray<AActor*> SirenLights_AsActors;
	UGameplayStatics::GetAllActorsOfClass(this, AElectric_Siren_SpotLight::StaticClass(), SirenLights_AsActors);

	for (AActor* SirenLight_AsActor : SirenLights_AsActors)
	{
		AElectric_Siren_SpotLight* SirenLight = Cast<AElectric_Siren_SpotLight>(SirenLight_AsActor);
		if (SirenLight) {
			SirensCache.AddUnique(SirenLight);
		}
	}
}

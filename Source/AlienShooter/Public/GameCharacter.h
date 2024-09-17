// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "GameCharacter.generated.h"

class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, Killer);

UCLASS()
class ALIENSHOOTER_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGameCharacter();

protected:
	virtual void BeginPlay() override;

	FOnDeath OnDeath;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSoftObjectPtr<UNiagaraSystem> DeathParticleSystem;

	ELifeStatus LifeStatus = ELifeStatus::ELS_Alive;
};

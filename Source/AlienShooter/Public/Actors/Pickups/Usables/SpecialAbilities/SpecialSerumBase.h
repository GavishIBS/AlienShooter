// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/UsableActor.h"
#include "Interfaces/SpecialSerumInterface.h"
#include "SpecialAbilities.h"
#include "SpecialSerumBase.generated.h"

/**
 * 
 */
UCLASS()
class ALIENSHOOTER_API ASpecialSerumBase : public AUsableActor, public ISpecialSerumInterface
{
	GENERATED_BODY()
	
public:
	ASpecialSerumBase();
	virtual bool UseItem(ACharacter* User) override;

	//virtual bool ApplyEffect(ACharacter* InCharacter) override;

	bool (*EffectImplementation)(ACharacter* InCharacter);

protected:
	virtual void BeginPlay() override;
	virtual bool CanUse(ACharacter* User, FHitResult& HitResult) override;

	UPROPERTY(EditAnywhere, Category = "Data | Effects")
	FSpecialSerumEffectData EffectData;

public:
	//FORCEINLINE virtual FSpecialSerumEffectData GetSerumEffectsData() const override { return EffectData; }
};

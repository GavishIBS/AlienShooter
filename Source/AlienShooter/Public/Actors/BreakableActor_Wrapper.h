// // Copyright (c) 2024 Gavish Bansal// // All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor_Wrapper.generated.h"

UCLASS()
class ALIENSHOOTER_API ABreakableActor_Wrapper : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor_Wrapper();
	virtual float GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType = nullptr) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<class ABreakableActor> BreakableActorClass;
};

// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletShell.generated.h"

UCLASS()
class ALIENSHOOTER_API ABulletShell : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletShell();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyBulletShell();
	
	UPROPERTY(VisibleDefaultsOnly, Category=Components)
	TObjectPtr<UStaticMeshComponent> ShellMesh;
	
	UPROPERTY(EditDefaultsOnly, Category=Ejection)
	float ShellEjectionImpulse = 5.f;

	UPROPERTY(EditDefaultsOnly, Category=Audio)
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, Category=Audio)
	TObjectPtr<USoundAttenuation> SoundAttenuation;

	UPROPERTY(EditDefaultsOnly, Category=Audio)
	TObjectPtr<USoundConcurrency> SoundConcurrency;

	UPROPERTY(EditDefaultsOnly, Category=Properties)
	float DestroyDelay = 1.f;

	FTimerHandle ShellDestroyHandle;
};

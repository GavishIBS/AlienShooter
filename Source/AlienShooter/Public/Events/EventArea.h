// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventArea.generated.h"

class UBoxComponent;
class UBillboardComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnterArea, AActor*, OtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExitArea, AActor*, OtherActor);

UCLASS()
class ALIENSHOOTER_API AEventArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AEventArea();

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnEnterArea OnEnterArea;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnExitArea OnExitArea;

protected:
	virtual void BeginPlay() override;
	virtual void BindCallbacks();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	TObjectPtr<UBoxComponent> OverlapArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBillboardComponent> Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Properties)
	FName TagToLook = FName("Player");
};

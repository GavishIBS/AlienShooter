// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "Characters/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/RadioPhoneComponent.h"
//#include "Components/ObjectiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Player");

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 1000.f;
	//CameraBoom->bUsePawnControlRotation

	CameraBoom_Secondary = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom_Secondary"));
	CameraBoom_Secondary->SetupAttachment(GetRootComponent());
	CameraBoom_Secondary->TargetArmLength = 1000.f;

	CameraBoomTopPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CameraBoomTopPoint"));
	CameraBoomTopPoint->SetupAttachment(CameraBoom_Secondary);

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	CameraOriginalLocation_Rel = ViewCamera->GetRelativeLocation();

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	RadioPhoneComponent = CreateDefaultSubobject<URadioPhoneComponent>(TEXT("RadioPhoneComponent"));
	RadioPhoneComponent->SetupAttachment(GetMesh());
	RadioPhoneComponent->SetObjectiveComponent(CreateDefaultSubobject<UObjectiveComponent>(TEXT("ObjectiveComponent")));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MovementType == EMovementType::EMT_WALK ?
		GetCharacterMovement()->MaxWalkSpeedCrouched : GetCharacterMovement()->MaxCustomMovementSpeed;

	//CameraBoomTopPoint->SetWorldRotation(ViewCamera->GetComponentRotation());
	//CameraBoomTopPoint->SetWorldLocation(ViewCamera->GetComponentLocation());
}

void ABaseCharacter::Tick(float DeltaTime)
{
	ViewCamera->SetRelativeLocation(FMath::VInterpTo(ViewCamera->GetRelativeLocation(), CameraTargetLocation_Rel, DeltaTime, 3.f));
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (InventoryComponent)
		InventoryComponent->InitInventory();
}

float ABaseCharacter::GetHit(const FVector& HitLocation, const float& Damage, const float& RandomizationFactor, AActor* InDamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EGetHitReturnType* OutGetHitReturnType)
{
	return IHitInterface::GetHit(HitLocation, Damage, RandomizationFactor, InDamageCauser, DamageTypeClass);
}

void ABaseCharacter::DoCameraShake(const FVector IntensityFactor, const float ShakeTime, const bool bShakeInDir, const FVector Direction)
{
	CameraShakeIntensity = IntensityFactor;
	bShakeInDirection = bShakeInDir;
	ShakeDirection = Direction;
	if (!bShakeInDirection) {
		GetWorldTimerManager().SetTimer(CameraShakeTimerHandle, this, &ABaseCharacter::GenerateRandomCameraLocation, 0.05, true, 0);
	}
	else {
		CameraTargetLocation_Rel = CameraOriginalLocation_Rel + FVector(
			CameraShakeIntensity.X * ShakeDirection.X,
			CameraShakeIntensity.Y * ShakeDirection.Y,
			CameraShakeIntensity.Z * ShakeDirection.Z
		);
	}
	GetWorldTimerManager().SetTimer(CameraShakeStopTimerHandle, this, &ABaseCharacter::StopCameraShake, ShakeTime, false, ShakeTime);
}

void ABaseCharacter::Rebirth(const FTransform& RebirthTransform)
{
	SetActorTransform(RebirthTransform);
	if (AttributeComponent) {
		AttributeComponent->ReInitComponent();
	}
	SetActorTickEnabled(true);
	EnableInput(Cast<APlayerController>(GetController()));
	LifeStatus = ELifeStatus::ELS_Alive;
}

void ABaseCharacter::TriggerSelfDialogue(const EReactionType InReactionType)
{
	static bool bPlaying = false;   // TEST // TEMPORARY // DEBUG

	if (bPlaying) return;  // TEMPORARY

	for (FReactionDialogues& RD : ReactionDialoguesData)
	{
		if (RD.TriggerChancePercent < UKismetMathLibrary::RandomIntegerInRange(1, 100)) continue;
		if (RD.Reaction == InReactionType && RD.DialogueAudios.Num() > 0)
		{
			TObjectPtr<USoundBase> SelectedAudio = RD.DialogueAudios[UKismetMathLibrary::RandomIntegerInRange(0, RD.DialogueAudios.Num() - 1)];
			UGameplayStatics::PlaySound2D(this, SelectedAudio);
			// TEMPORARY begin
			bPlaying = true;
			FTimerHandle CanPlayAnotherDialogueTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(CanPlayAnotherDialogueTimerHandle, FTimerDelegate::CreateLambda([this] {
				bPlaying = false;
			}), 2.2f, false, 2.2f);
			// TEMPORARY end
			break;
		}
	}
}

void ABaseCharacter::GenerateRandomCameraLocation()
{
	if (!bShakeInDirection) {
		CameraTargetLocation_Rel = CameraOriginalLocation_Rel + FVector(
			FMath::RandRange(-15, 20) * CameraShakeIntensity.X,
			FMath::RandRange(-20, 20) * CameraShakeIntensity.Y,
			FMath::RandRange(-5, 10) * CameraShakeIntensity.Z
		);
	}
	/*
	else {
		CameraTargetLocation_Rel = CameraOriginalLocation_Rel + FVector(
			CameraShakeIntensity.X * ShakeDirection.X,
			CameraShakeIntensity.Y * ShakeDirection.Y,
			CameraShakeIntensity.Z * ShakeDirection.Z
		);
	}
	*/
}

void ABaseCharacter::StopCameraShake()
{
	GetWorldTimerManager().ClearTimer(CameraShakeTimerHandle);
	CameraTargetLocation_Rel = CameraOriginalLocation_Rel;
	GetWorldTimerManager().ClearTimer(CameraShakeStopTimerHandle);
}

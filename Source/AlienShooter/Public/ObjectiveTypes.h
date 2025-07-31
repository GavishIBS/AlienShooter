// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveTypes.generated.h"

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	NONE = 0, GOTO_POINT = 1, EVENT = 2
};

USTRUCT(BlueprintType)
struct FObjective
{
	GENERATED_BODY()

	FObjective() = default;
	FObjective(int32 InKey, FString Desc, bool bOpt, EObjectiveType Type, bool bRem) :
		Key(InKey), ObjectiveDesc(Desc), bOptional(bOpt), ObjectiveType(Type) {}

	/* TODO: Add a unique key to check objective/reminding priority - before adding objective to list check if a objective/reminder with same key exists
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Key = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	FString ObjectiveDesc = FString("Objective text");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	bool bOptional = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	EObjectiveType ObjectiveType = EObjectiveType::NONE;
};

USTRUCT(BlueprintType)
struct FReminderInfo
{
	GENERATED_BODY()

	FReminderInfo() = default;
	FReminderInfo(int32 InKey, FString RemingText, class USoundWave* RemindAudio) :
		Key(InKey), RemindingText(RemingText), RemindingAudio(RemindAudio) {
	}

	/* TODO: Add a unique key to check objective/reminding priority - before adding objective to list check if a objective/reminder with same key exists
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Key = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString RemindingText = FString("Reminding Objective");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class USoundWave> RemindingAudio = nullptr;
};

USTRUCT(BlueprintType)
struct FRemindableObjective
{
	GENERATED_BODY()

	FRemindableObjective() = default;
	FRemindableObjective(FObjective InObjectiveData, FString RemingText, class USoundWave* RemindAudio) :
		ObjectiveData(InObjectiveData), RemindingText(RemingText), RemindingAudio(RemindAudio) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FObjective ObjectiveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString RemindingText = FString("Reminding Objective");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class USoundWave> RemindingAudio = nullptr;

	explicit operator FObjective() const {
		return ObjectiveData;
	}

	explicit operator FReminderInfo() const {
		return { ObjectiveData.Key, RemindingText, RemindingAudio };
	}
};

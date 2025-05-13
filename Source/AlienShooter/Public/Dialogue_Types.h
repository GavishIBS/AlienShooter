// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue_Types.generated.h"

class USoundWave;

USTRUCT(BlueprintType)
struct FDialogueData
{
	GENERATED_BODY()


	FString Dialogue = FString();
	
	UPROPERTY(EditAnywhere, Category=Data)
	USoundWave* DialogueAudio = nullptr;
	
	UPROPERTY(EditAnywhere, Category=Data)
	bool bIsReply = false;
};

USTRUCT(BlueprintType)
struct FDialogueLog
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	//FName OtherNPC_Name = FName("Injured Guard");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName SpeakerName = FName("Injured Guard");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString DialogueText = FString("Sample dialogue text");
};

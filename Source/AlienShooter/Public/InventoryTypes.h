// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FName ItemName = FName();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString ItemDescription = FString();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UTexture2D* IconTexture = nullptr;

	UPROPERTY(EditAnywhere, Category = "Data")
	TSubclassOf<AActor> ItemClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Data")
	bool bUsable = false;
};

USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FItemData ItemData{};

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bEmpty = true;
};

USTRUCT(BlueprintType)
struct FItemProperties
{
	GENERATED_BODY();

public:
	FItemProperties() {};
	FItemProperties(FName InAttachSocketName) : AttachmentSocketName(InAttachSocketName) {};

	UPROPERTY(EditAnywhere, Category = "Properties")
	FName AttachmentSocketName = FName("ItemSocket");
};

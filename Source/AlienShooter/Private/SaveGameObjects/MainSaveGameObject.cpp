// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "SaveGameObjects/MainSaveGameObject.h"
#include "Kismet/GameplayStatics.h"

UMainSaveGameObject::UMainSaveGameObject()
{
	SaveSlotName = TEXT("MainSaveSlot");
	UserIndex = 0;
}

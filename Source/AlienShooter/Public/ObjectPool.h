// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"

template <typename T>
class UObjectPool
{
public:
	UObjectPool() = default;
	UObjectPool(UWorld* InWorld, TSubclassOf<T> InObjectClass, int InPoolSize) :
		World(InWorld), PoolObject_Class(InObjectClass), PoolSize(InPoolSize) {}
	~UObjectPool() {}

	int CreatePool() {
		//if (std::is_base_of_v<T, APooledActor> == false) {
			//return -1;
		//}
		for (int i = 0; i < PoolSize; i++) {
			T* Spawned = World->SpawnActor<T>(PoolObject_Class, FActorSpawnParameters());
			Pool.Add(Spawned);
			APooledActor* PooledActor = (APooledActor*)Spawned;
			if (PooledActor) {
				PooledActor->SetInUse(false);
				PooledActor->SetSpawnedByPool(true);
			}
		}
		return PoolSize;
	}
	void DestroyPool() {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("Destroying pool!"));
		for (int i = 0; i < Pool.Num(); i++) {
			if (Pool[i] != nullptr)
			{
				APooledActor* PooledActor = (APooledActor*)Pool[i];
				if (PooledActor)
				{
					PooledActor->Destroy();
				}
			}
		}
		Pool.Empty();
	}

	T* SpawnObjectFromPool() {
		for (int i = 0; i < Pool.Num(); i++) {
			if (Pool[i] != nullptr) {
				APooledActor* PooledActor = (APooledActor*)Pool[i];
				if (PooledActor)
				{
					if (PooledActor->IsInUse() == false)
					{
						//PooledActor->SetInUse(true);  // Should be called after setting primary settings such as location
						// thus, called explicitly
						return Pool[i];
					}
				}
			}
		}
		return nullptr;
	}
	void ReturnObjectToPool(APooledActor* InPooledActor) {
		InPooledActor->SetInUse(false);
	}

private:
	UWorld* World;
	TSubclassOf<T> PoolObject_Class;
	int PoolSize = 25;

	TArray<T*> Pool;
};

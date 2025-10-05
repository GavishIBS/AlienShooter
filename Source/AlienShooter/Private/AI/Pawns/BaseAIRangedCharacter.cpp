// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.


#include "AI/Pawns/BaseAIRangedCharacter.h"

ABaseAIRangedCharacter::ABaseAIRangedCharacter()
{
}

void ABaseAIRangedCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAIRangedCharacter::SpawnRangedAttack()
{
	if (Attack_ProjectileClass == nullptr) return;

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	FTransform SpawnTransform = GetMesh()->GetSocketTransform(RangedAttackSpawnSocketName, ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.Instigator = this;
	ActorSpawnParams.Owner = this;
	World->SpawnActor<AEnemyAttackProjectile>(Attack_ProjectileClass, SpawnTransform, ActorSpawnParams);
}

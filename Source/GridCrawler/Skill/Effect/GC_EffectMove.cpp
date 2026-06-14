// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Effect/GC_EffectMove.h"

#include "Game\GC_GridManager.h"
#include "Character\GC_Character.h"

void UGC_EffectMove::ApplyEffect(const FSkillContext& Context)
{
	UE_LOG(LogTemp, Log, TEXT("Effect : Move"));

	AGC_GridManager* GridManager = Context.GridManager;
	AGC_Character* User = Context.User;
	AGC_Character* Target = Context.Target;

	check(Target);

	FGridCoord Origin = Target->GetGridCoord();
	ECharacterDirection UserDir = User->GetCharacterDirection();

	TArray<FGridCoord> TargetPath;
	for (FGridCoord Coord : PathOffset)
	{
		TargetPath.Add(Origin + GridManager->RotateCoordOffset(Coord, UserDir));
	}

	Target->RequestForcedMove(User == Target, TargetPath, MoveSpeed);
}

float UGC_EffectMove::GetEffectValue(const FSkillContext& Context)
{
	return 0.0f;
}

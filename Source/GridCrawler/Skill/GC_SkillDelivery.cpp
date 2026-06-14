// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GC_SkillDelivery.h"

#include "Character\GC_Character.h"

bool UGC_SkillDelivery::IsValidTargetInArea(const FSkillContext& Context, TArray<FGridCoord>& Area)
{
	check(Context.EffectIndex < Effects.Num());

	AGC_GridManager* GridManager = Context.GridManager;
	AGC_Character* User = Context.User;
	UGC_SkillEffect* Effect = Effects[Context.EffectIndex];

	TSet<AGC_Character*> Targets;

	for (const FGridCoord& Coord : Area)
	{
		AGC_Character* Target = Cast<AGC_Character>(GridManager->GetOccupyingActor(Coord));

		if (!Target) continue;
		if (Targets.Contains(Target)) continue;

		if (IsValidTarget(User, Target, Effect->TargetType))
		{
			return true;
		}
		else
		{
			Targets.Add(Target);
		}
	}

	return false;
}

bool UGC_SkillDelivery::IsValidTarget(AGC_Character* User, AGC_Character* Target, EEffectTarget TargetType)
{
	if (TargetType == EEffectTarget::Ally)
	{
		return User->GetTeam() == Target->GetTeam();
	}
	else if (TargetType == EEffectTarget::Opponent)
	{
		return User->GetTeam() != Target->GetTeam();
	}

	return true;
}
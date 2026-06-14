// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_BattleManager.h"

#include "Game\GC_GridManager.h"
#include "Game\GC_IndicatorManager.h"
#include "Character\GC_Character.h"
#include "Skill\GC_SkillData.h"
#include "Skill\GC_SkillDelivery.h"

// Sets default values
AGC_BattleManager::AGC_BattleManager()
{

}

void AGC_BattleManager::Init(AGC_GridManager* InGridManager, AGC_IndicatorManager* InIndicatorManager)
{
	GridManager = InGridManager;
	IndicatorManager = InIndicatorManager;
}

bool AGC_BattleManager::IsTargetInRange(AGC_Character* User, const UGC_SkillData* Skill)
{
	FSkillContext Context;
	Context.GridManager = GridManager;
	Context.User = User;

	for (int32 i = 0; i < Skill->Deliveries.Num(); ++i)
	{
		TArray<FGridCoord> EffectArea;
		Skill->Deliveries[i]->GetSkillIndicator(Context, EffectArea);

		Context.EffectIndex = i;
		if (Skill->Deliveries[i]->IsValidTargetInArea(Context, EffectArea))
		{
			return true;
		}
	}

	return false;
}

int32 AGC_BattleManager::IndicateSkillEffect(AGC_Character* User, const UGC_SkillData* Skill)
{
	if (!User || !Skill) return -1;

	FSkillContext Context;
	Context.GridManager = GridManager;
	Context.User = User;
	Context.SkillInstanceID = GenerateID();

	for (int32 i = 0; i < Skill->Deliveries.Num(); ++i)
	{
		TArray<FGridCoord> EffectArea;

		Skill->Deliveries[i]->GetSkillIndicator(Context, EffectArea);

		Context.EffectArea = EffectArea;

		if (EffectArea.Num() > 0)
		{
			if (User->GetTeam() == ETeam::Player)
			{
				IndicatorManager->CreatePlayerIndicator(FIntPoint(Context.SkillInstanceID, i), EffectArea);
			}
			else if (User->GetTeam() == ETeam::Enemy)
			{
				IndicatorManager->CreateEnemyIndicator(FIntPoint(Context.SkillInstanceID, i), EffectArea);
			}
		
			Context.EffectIndex = i;
			ActiveSkills.Add(FIntPoint(Context.SkillInstanceID, i), Context);
		}
	}

	return Context.SkillInstanceID;
}

void AGC_BattleManager::RemoveSkillIndicator(int32 SkillInstanceID, int32 StepIndex)
{
	FIntPoint Key(SkillInstanceID, StepIndex);

	if (ActiveSkills.Contains(Key))
	{
		IndicatorManager->RemoveIndicator(Key);
	}
}

void AGC_BattleManager::ProcessSkillEffect(const UGC_SkillData* Skill, int32 SkillInstanceID, int32 StepIndex)
{
	FIntPoint Key(SkillInstanceID, StepIndex);

	if (ActiveSkills.Contains(Key))
	{
		FSkillContext Context = ActiveSkills[Key];
		Context.EffectIndex = StepIndex;

		Skill->Deliveries[StepIndex]->Execute(Context);

		RemoveSkillIndicator(SkillInstanceID, StepIndex);

		ActiveSkills.Remove(Key);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO ActiveSkill's Key"));
	}
}
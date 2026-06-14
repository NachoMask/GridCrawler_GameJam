// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Game\GC_Grid.h"
#include "Skill\GC_Skill.h"

#include "GC_BattleManager.generated.h"

class AGC_GridManager;
class AGC_IndicatorManager;
class AGC_Character;
class UGC_SkillData;

UCLASS()
class GRIDCRAWLER_API AGC_BattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGC_BattleManager();

public:
	void Init(AGC_GridManager* InGridManager, AGC_IndicatorManager* InIndicatorManager);

	bool IsTargetInRange(AGC_Character* User, const UGC_SkillData* Skill);

	int32 IndicateSkillEffect(AGC_Character* User, const UGC_SkillData* Skill);

	void RemoveSkillIndicator(int32 SkillInstanceID, int32 StepIndex);

	void ProcessSkillEffect(const UGC_SkillData* Skill, int32 SkillInstanceID, int32 StepIndex);

protected:
	int32 GenerateID()
	{
		return ++CurrentSkillID;
	}

protected:
	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_GridManager> GridManager = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_IndicatorManager> IndicatorManager = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|ActiveSkills")
	TMap<FIntPoint, FSkillContext> ActiveSkills;

	int32 CurrentSkillID = 0;
};

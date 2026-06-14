// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Game\GC_Grid.h"
#include "Skill\GC_Skill.h"
#include "Skill\GC_SkillEffect.h"

#include "GC_SkillDelivery.generated.h"

UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class GRIDCRAWLER_API UGC_SkillDelivery : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Execute(const FSkillContext& Context)
		PURE_VIRTUAL(UGC_SkillDelivery::Execute, );

	virtual void GetSkillIndicator(const FSkillContext& Context, TArray<FGridCoord>& Result)
		PURE_VIRTUAL(UGC_SkillDelivery::GetSkillIndicator, );

	bool IsValidTargetInArea(const FSkillContext& Context, TArray<FGridCoord>& Area);

protected:
	bool IsValidTarget(AGC_Character* User, AGC_Character* Target, EEffectTarget TargetType);

public:
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UGC_SkillEffect>> Effects;
};

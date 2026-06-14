// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Skill\GC_Skill.h"

#include "GC_SkillEffect.generated.h"

class AGC_Character;

UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class GRIDCRAWLER_API UGC_SkillEffect : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(const FSkillContext& Context)
		PURE_VIRTUAL(UGC_SkillEffect::ApplyEffect, );

	virtual float GetEffectValue(const FSkillContext& Context)
		PURE_VIRTUAL(UGC_SkillEffect::GetEffectValue, return 0.f;);

public:
	UPROPERTY(EditAnywhere)
	EEffectTarget TargetType;

	bool bFinished = false;
};

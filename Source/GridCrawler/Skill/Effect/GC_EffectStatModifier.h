// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GC_SkillEffect.h"

#include "Character\GC_CharacterStat.h"

#include "GC_EffectStatModifier.generated.h"

UCLASS()
class GRIDCRAWLER_API UGC_EffectStatModifier : public UGC_SkillEffect
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(const FSkillContext& Context) override;

	virtual float GetEffectValue(const FSkillContext& Context) override;

protected:
	UPROPERTY(EditAnywhere)
	ECharacterStatType StatType;

	UPROPERTY(EditAnywhere)
	EStatModifierOp Operation;

	UPROPERTY(EditAnywhere)
	float Value = 0.f;

	UPROPERTY(EditAnywhere)
	float Duration = 0.f;
};

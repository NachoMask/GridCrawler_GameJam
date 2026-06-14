// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GC_SkillEffect.h"
#include "GC_EffectDamage.generated.h"

UCLASS()
class GRIDCRAWLER_API UGC_EffectDamage : public UGC_SkillEffect
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(const FSkillContext& Context) override;

	virtual float GetEffectValue(const FSkillContext& Context) override;

public:
	UPROPERTY(EditAnywhere)
	float BaseValue = 0.f;

	UPROPERTY(EditAnywhere)
	float AttackRatio = 1.f;
};

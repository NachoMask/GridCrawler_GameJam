// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GC_SkillEffect.h"

#include "Game\GC_Grid.h"

#include "GC_EffectMove.generated.h"

UCLASS()
class GRIDCRAWLER_API UGC_EffectMove : public UGC_SkillEffect
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(const FSkillContext& Context) override;

	virtual float GetEffectValue(const FSkillContext& Context) override;

public:
	UPROPERTY(EditAnywhere)
	TArray<FGridCoord> PathOffset;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 0.f;
};

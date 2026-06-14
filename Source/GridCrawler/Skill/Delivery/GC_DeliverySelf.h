// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GC_SkillDelivery.h"
#include "GC_DeliverySelf.generated.h"

UCLASS()
class GRIDCRAWLER_API UGC_DeliverySelf : public UGC_SkillDelivery
{
	GENERATED_BODY()
	
public:
	virtual void Execute(const FSkillContext& Context) override;

	virtual void GetSkillIndicator(const FSkillContext& Context, TArray<FGridCoord>& Result) override;
};

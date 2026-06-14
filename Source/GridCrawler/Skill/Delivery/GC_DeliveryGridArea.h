// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GC_SkillDelivery.h"

#include "Game\GC_Grid.h"

#include "GC_DeliveryGridArea.generated.h"

UCLASS()
class GRIDCRAWLER_API UGC_DeliveryGridArea : public UGC_SkillDelivery
{
	GENERATED_BODY()
	
public:
	virtual void Execute(const FSkillContext& Context) override;

	virtual void GetSkillIndicator(const FSkillContext& Context, TArray<FGridCoord>& Result) override;

protected:
	TArray<FGridCoord> GetArea(AGC_GridManager* GridManager, const FGridCoord& Origin, ECharacterDirection Dir);

	TArray<AGC_Character*> GetTargetInArea(AGC_GridManager* GridManager, const TArray<FGridCoord>& EffectArea);

public:
	UPROPERTY(EditAnywhere)
	TArray<FGridCoord> Offsets;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill\Delivery\GC_DeliveryGridArea.h"

#include "Game\GC_GridManager.h"
#include "Character\GC_Character.h"

void UGC_DeliveryGridArea::Execute(const FSkillContext& Context)
{
	//UE_LOG(LogTemp, Log, TEXT("Delivery : GridArea"));

	AGC_GridManager* GridManager = Context.GridManager;
	AGC_Character* User = Context.User;

	const TArray<FGridCoord> Area = Context.EffectArea;
	const TArray<AGC_Character*> Targets = GetTargetInArea(GridManager, Area);

	for (AGC_Character* Target : Targets)
	{
		for (UGC_SkillEffect* Effect : Effects)
		{
			if (IsValidTarget(User, Target, Effect->TargetType))
			{
				FSkillContext EffectContext = Context;
				EffectContext.Target = Target;

				Effect->ApplyEffect(EffectContext);
			}
		}
	}
}

void UGC_DeliveryGridArea::GetSkillIndicator(const FSkillContext& Context, TArray<FGridCoord>& Result)
{
	AGC_GridManager* GridManager = Context.GridManager;
	AGC_Character* User = Context.User;

	Result = GetArea(GridManager, User->GetGridCoord(), User->GetCharacterDirection());
}

TArray<FGridCoord> UGC_DeliveryGridArea::GetArea(AGC_GridManager* GridManager, const FGridCoord& Origin, ECharacterDirection Dir)
{
	TArray<FGridCoord> Result;

	for (const FGridCoord& Offset : Offsets)
	{
		FGridCoord Coord = Origin + GridManager->RotateCoordOffset(Offset, Dir);

		Result.Add(Coord);
	}

	return Result;
}

TArray<AGC_Character*> UGC_DeliveryGridArea::GetTargetInArea(AGC_GridManager* GridManager, const TArray<FGridCoord>& Area)
{
	TSet<AGC_Character*> Result;

	for (const FGridCoord& Coord : Area)
	{
		if (AGC_Character* Target = Cast<AGC_Character>(GridManager->GetOccupyingActor(Coord)))
		{
			Result.Add(Target);
		}
	}

	return Result.Array();
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Delivery/GC_DeliverySelf.h"

#include "Character\GC_Character.h"

void UGC_DeliverySelf::Execute(const FSkillContext& Context)
{
	//UE_LOG(LogTemp, Log, TEXT("Delivery : Self"));

	AGC_Character* User = Context.User;

	for (UGC_SkillEffect* Effect : Effects)
	{
		FSkillContext EffectContext = Context;
		EffectContext.Target = User;

		Effect->ApplyEffect(EffectContext);
	}
}

void UGC_DeliverySelf::GetSkillIndicator(const FSkillContext& Context, TArray<FGridCoord>& Result)
{
	AGC_Character* User = Context.User;
	Result.Add(User->GetGridCoord());
}

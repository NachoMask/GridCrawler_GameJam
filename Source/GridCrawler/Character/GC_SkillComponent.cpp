// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC_SkillComponent.h"

#include "Character\GC_Character.h"
#include "Skill\GC_SkillData.h"

UGC_SkillComponent::UGC_SkillComponent()
{
	Skills.SetNum(MaxSkillNum + 1);
	SkillExecutedTime.SetNum(MaxSkillNum + 1);
}

void UGC_SkillComponent::InitSkills(const TArray<const UGC_SkillData*>& InSkills)
{
	for (int32 i = 0; i < InSkills.Num(); ++i)
	{
		ChangeSkill(i, InSkills[i]);
	}
}

void UGC_SkillComponent::ChangeSkill(int32 SkillIndex, const UGC_SkillData* InSkill)
{
	if (Skills.IsValidIndex(SkillIndex))
	{
		Skills[SkillIndex] = InSkill;
		SkillExecutedTime[SkillIndex] = -9999.f;

		UTexture2D* SkillIcon = nullptr;
		FName SkillName;

		if (InSkill)
		{
			SkillIcon = InSkill->SkillIcon;
			SkillName = InSkill->SkillName;

			FSkillContext Context;
			Context.User = Cast<AGC_Character>(GetOwner());
		}

		OnSkillChange.Broadcast(SkillIndex, SkillIcon);
	}
}

const UGC_SkillData* UGC_SkillComponent::GetSkillData(int32 Idx) const
{
	if (Skills.IsValidIndex(Idx))
	{
		return Skills[Idx];
	}

	return nullptr;
}

void UGC_SkillComponent::SetSkillInUse(int32 Idx)
{
	OnSkillInUse.Broadcast(Idx);
}

float UGC_SkillComponent::GetSkillExecutedTime(int32 Idx) const
{
	if (Idx >= SkillExecutedTime.Num()) return -9999.f;

	return SkillExecutedTime[Idx];
}

void UGC_SkillComponent::SetSkillExecutedTime(int32 Idx, float Time)
{
	if (Idx >= SkillExecutedTime.Num()) return;

	SkillExecutedTime[Idx] = Time;

	float Cooldown = GetSkillData(Idx)->SkillCooldown;

	OnSkillCooldown.Broadcast(Idx, Cooldown);
}

bool UGC_SkillComponent::IsSkillInCooldown(int32 Idx, float Time)
{
	if (SkillExecutedTime.IsValidIndex(Idx))
	{
		return Time - GetSkillExecutedTime(Idx) < GetSkillData(Idx)->SkillCooldown;
	}
	
	return true;
}

void UGC_SkillComponent::BroadcastCurrentSkills()
{
	for (int32 i = 0; i < Skills.Num(); ++i)
	{
		UTexture2D* SkillIcon = nullptr;
		FName SkillName;

		if (Skills[i])
		{
			SkillIcon = Skills[i]->SkillIcon;
			SkillName = Skills[i]->SkillName;

			FSkillContext Context;
			Context.User = Cast<AGC_Character>(GetOwner());
		}

		OnSkillChange.Broadcast(i, SkillIcon);
	}
}

void UGC_SkillComponent::BroadcastCurrentSkills(float ChangedAttack)
{
	for (int32 i = 0; i < Skills.Num(); ++i)
	{
		UTexture2D* SkillIcon = nullptr;
		FName SkillName;

		if (Skills[i])
		{
			SkillIcon = Skills[i]->SkillIcon;
			SkillName = Skills[i]->SkillName;

			FSkillContext Context;
			Context.User = Cast<AGC_Character>(GetOwner());
		}

		OnSkillChange.Broadcast(i, SkillIcon);
	}
}

int32 UGC_SkillComponent::GetNextEmptySkillSlot() const
{
	for (int32 i = 0; i < Skills.Num(); ++i)
	{
		if (Skills[i] == nullptr)
			return i;
	}

	return -1;
}

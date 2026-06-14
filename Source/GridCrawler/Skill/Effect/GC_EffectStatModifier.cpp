// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Effect/GC_EffectStatModifier.h"

#include "Character\GC_Character.h"
#include "Character\GC_CharacterStatComponent.h"

void UGC_EffectStatModifier::ApplyEffect(const FSkillContext& Context)
{
	AGC_Character* Target = Context.Target;

	check(Target);

	FStatModifier Modifier;
	Modifier.StatType = StatType;
	Modifier.Operation = Operation;
	Modifier.Value = Value;
	Modifier.Duration = Duration;

	Target->GetStatComponent()->AddStatModifier(Modifier);
}

float UGC_EffectStatModifier::GetEffectValue(const FSkillContext& Context)
{
	return 0.0f;
}

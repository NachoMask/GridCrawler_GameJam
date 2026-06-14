// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Effect/GC_EffectDamage.h"

#include "Character\GC_Character.h"

void UGC_EffectDamage::ApplyEffect(const FSkillContext& Context)
{
	float Damage = 0.f;

	AGC_Character* User = Context.User;
	AGC_Character* Target = Context.Target;

	check(User);
	check(Target);

	Damage = (User->GetStatAttack() * AttackRatio) + BaseValue;

	Target->ApplyDamage(Damage);
}

float UGC_EffectDamage::GetEffectValue(const FSkillContext& Context)
{
	float Damage = 0.f;

	AGC_Character* User = Context.User;
	check(User);

	Damage = (User->GetStatAttack() * AttackRatio) + BaseValue;

	return Damage;
}

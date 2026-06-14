// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC_CharacterStatComponent.h"

#include "Character\GC_Character.h"

UGC_CharacterStatComponent::UGC_CharacterStatComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UGC_CharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UGC_CharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = ActiveModifiers.Num() - 1; i >= 0; --i)
	{
		FStatModifier& Modifier = ActiveModifiers[i];

		// Duration <= 0.f is Infinity Modifier
		if (Modifier.Duration > 0.f)
		{
			Modifier.RemainTime -= DeltaTime;
			
			if (Modifier.RemainTime <= 0.f)
			{
				float OldMaxHp = GetMaxHp();
				ECharacterStatType RemoveStatType = Modifier.StatType;

				ActiveModifiers.RemoveAt(i);

				switch (RemoveStatType)
				{
				case ECharacterStatType::MaxHp:
				{
					RefreshHpByMaxHpChange(OldMaxHp, GetMaxHp());

					OnHpChanged.Broadcast(GetCurrentHp());
					OnMaxHpChanged.Broadcast(GetMaxHp());
					break;
				}
				case ECharacterStatType::Attack:
				{
					OnAttackChanged.Broadcast(GetAttack());
					break;
				}
				case ECharacterStatType::Armor:
				{
					OnArmorChanged.Broadcast(GetArmor());
					break;
				}
				case ECharacterStatType::MoveSpeed:
				{
					OnMoveSpeedChanged.Broadcast(GetMoveSpeed());
					break;
				}
				}
			}
		}
	}
}

void UGC_CharacterStatComponent::InitStat(const FCharacterBaseStats& InStats)
{
	// Separate InStats's Stat in float units
	const float* const InStatsPtr = reinterpret_cast<const float* const>(&InStats);

	// Reinterpret BaseStat as float variable pointer
	float* BaseStatPtr = reinterpret_cast<float*>(&BaseStat);
	// Check Stat Count of BaseStat
	int32 StatNum = sizeof(FCharacterBaseStats) / sizeof(float);

	// Apply InStats's Stat to BaseStat
	for (int32 i = 0; i < StatNum; ++i)
	{
		BaseStatPtr[i] = InStatsPtr[i];
	}

	SetCurrentHp(GetMaxHp());
}

void UGC_CharacterStatComponent::AddStatModifier(const FStatModifier& InModifier)
{
	float OldMaxHp = GetMaxHp();

	if (InModifier.StatType != ECharacterStatType::None)
	{
		FStatModifier Modifier = InModifier;
		Modifier.RemainTime = Modifier.Duration;

		ActiveModifiers.Add(Modifier);

		switch (InModifier.StatType)
		{
		case ECharacterStatType::MaxHp:
		{
			RefreshHpByMaxHpChange(OldMaxHp, GetMaxHp());

			OnHpChanged.Broadcast(GetCurrentHp());
			OnMaxHpChanged.Broadcast(GetMaxHp());
			break;
		}
		case ECharacterStatType::Attack:
		{
			OnAttackChanged.Broadcast(GetAttack());
			break;
		}
		case ECharacterStatType::Armor:
		{
			OnArmorChanged.Broadcast(GetArmor());
			break;
		}
		case ECharacterStatType::MoveSpeed:
		{
			OnMoveSpeedChanged.Broadcast(GetMoveSpeed());
			break;
		}
		}
	}
}

void UGC_CharacterStatComponent::SetCurrentHp(float NewCurrentHp)
{
	float OldCurrentHp = CurrentHp;
	CurrentHp = FMath::Clamp(NewCurrentHp, 0.f, GetMaxHp());

	if (CurrentHp - OldCurrentHp != 0)
	{
		OnHpChanged.Broadcast(CurrentHp);
	}

	if (CurrentHp < 1.f && OldCurrentHp >= 1.f)
	{
		OnHpZero.Broadcast();
	}
}

float UGC_CharacterStatComponent::ApplyDamage(float Damage)
{
	float ActualDamage = Damage;

	if (GetArmor() >= 0)
	{
		ActualDamage = Damage * (100 / (100 + GetArmor() * 2));
	}
	else
	{
		ActualDamage = Damage * (1 + (-GetArmor() / 100));
	}

	// UE_LOG(LogTemp, Warning, TEXT("DMG : %f -> %f"), ActualDamage, FMath::RoundToFloat(ActualDamage));
	
	ActualDamage = FMath::RoundToFloat(ActualDamage);

	SetCurrentHp(GetCurrentHp() - ActualDamage);

	return ActualDamage;
}

float UGC_CharacterStatComponent::ApplyHeal(float HealAmount)
{
	const float ActualHealAmount = FMath::Clamp(HealAmount, 0.f, HealAmount);

	SetCurrentHp(GetCurrentHp() + ActualHealAmount);

	return ActualHealAmount;
}

void UGC_CharacterStatComponent::BroadcastCurrentStats()
{
	OnHpChanged.Broadcast(GetCurrentHp());
	OnMaxHpChanged.Broadcast(GetMaxHp());
	OnAttackChanged.Broadcast(GetAttack());
	OnArmorChanged.Broadcast(GetArmor());
	OnMoveSpeedChanged.Broadcast(GetMoveSpeed());
}

float UGC_CharacterStatComponent::CalculateFinalStat(ECharacterStatType StatType, float BaseValue) const
{
	float AddSum = 0.f;
	float SubSum = 0.f;
	float MulProduct = 1.f;
	float DivFactor = 1.f;

	// Set Calculated Value According to Stat Type and Operator of Modifier
	for (const FStatModifier& Modifier : ActiveModifiers)
	{
		if (Modifier.StatType != StatType)
		{
			continue;
		}

		switch (Modifier.Operation)
		{
		case EStatModifierOp::Add:
			AddSum += Modifier.Value;
			break;
		case EStatModifierOp::Subtract:
			SubSum += Modifier.Value;
			break;
		case EStatModifierOp::Multiply:
			MulProduct *= Modifier.Value;
			break;
		case EStatModifierOp::Divide:
			DivFactor += Modifier.Value;
			break;
		}
	}

	// Clamp that Division Factor is not lower than 0
	DivFactor = FMath::Clamp(DivFactor, 0.0001f, DivFactor);

	// Apply AddSum and SubSum
	float FlatValue = BaseValue + AddSum - SubSum;

	// Clamp that MoveSpeed Value is not lower than MinMoveSpeed
	if (StatType == ECharacterStatType::MoveSpeed)
	{
		FlatValue = FMath::Clamp(FlatValue, MinMoveSpeed, FlatValue);
	}
	// Is Stat is not MoveSpeed, Clamp that Stat Value is not lower than 0
	else
	{
		// FlatValue = FMath::Clamp(FlatValue, 0.f, FlatValue);
	}

	// Apply MulProduct and DivFactor
	float Final = FlatValue * MulProduct / DivFactor;

	return Final;
}

void UGC_CharacterStatComponent::RefreshHpByMaxHpChange(float OldMaxHp, float NewMaxHp)
{
	// If OldMaxHp is lower than 0, Set CurrentHp to NewMaxHp
	if (OldMaxHp <= 0.f)
	{
		CurrentHp = NewMaxHp;
		return;
	}

	// Set CurrentHp to Previous Hp Percentage
	const float Ratio = CurrentHp / OldMaxHp;
	CurrentHp = FMath::Clamp(NewMaxHp * Ratio, 0.f, NewMaxHp);
}

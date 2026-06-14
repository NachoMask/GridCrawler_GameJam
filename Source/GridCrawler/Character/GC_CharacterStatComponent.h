// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Character\GC_CharacterStat.h"

#include "GC_CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHpChangedDelegate, float /*MaxHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttackChangedDelegate, float /*Attack*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnArmorChangedDelegate, float /*Armor*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveSpeedChangedDelegate, float /*Speed*/);

class AGC_Character;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDCRAWLER_API UGC_CharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGC_CharacterStatComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnMaxHpChangedDelegate OnMaxHpChanged;
	FOnAttackChangedDelegate OnAttackChanged;
	FOnArmorChangedDelegate OnArmorChanged;
	FOnMoveSpeedChangedDelegate OnMoveSpeedChanged;

	FORCEINLINE float GetMaxHp() const
	{
		return CalculateFinalStat(ECharacterStatType::MaxHp, BaseStat.MaxHp);
	}

	FORCEINLINE float GetAttack() const
	{
		return CalculateFinalStat(ECharacterStatType::Attack, BaseStat.Attack);
	}

	FORCEINLINE float GetArmor() const
	{
		return CalculateFinalStat(ECharacterStatType::Armor, BaseStat.Armor);
	}

	FORCEINLINE float GetMoveSpeed() const
	{
		return CalculateFinalStat(ECharacterStatType::MoveSpeed, BaseStat.MoveSpeed);
	}

	FORCEINLINE float GetAttackSpeed() const
	{
		return CalculateFinalStat(ECharacterStatType::AttackSpeed, BaseStat.AttackSpeed);
	}

	FORCEINLINE float GetCurrentHp() const
	{
		return CurrentHp;
	}

	void InitStat(const FCharacterBaseStats& InStats);

	void AddStatModifier(const FStatModifier& InModifier);

	void SetCurrentHp(float NewCurrentHp);

	float ApplyDamage(float Damage);

	float ApplyHeal(float HealAmount);

	void BroadcastCurrentStats();

protected:
	float CalculateFinalStat(ECharacterStatType StatType, float BaseValue) const;
	void RefreshHpByMaxHpChange(float OldMaxHp, float NewMaxHp);

protected:
	/** Stat */
	UPROPERTY(Transient, EditInstanceOnly, Category = "GC|Stat")
	FCharacterBaseStats BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "GC|Stat")
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "GC|Stat")
	TArray<FStatModifier> ActiveModifiers;

	UPROPERTY()
	bool bStatChanged = false;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Stat")
	float MinMoveSpeed = 100.f;
};

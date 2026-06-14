#pragma once

#include "CoreMinimal.h"
#include "GC_CharacterStat.generated.h"

UENUM(BlueprintType)
enum class ECharacterStatType : uint8
{
	None,
	MaxHp,
	Attack,
	Armor,
	MoveSpeed,
	AttackSpeed,
};

UENUM(BlueprintType)
enum class EStatModifierOp : uint8
{
	None,
	Add,
	Subtract,
	Multiply,
	Divide
};

USTRUCT(BlueprintType)
struct FCharacterBaseStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "GC")
	float MaxHp;

	UPROPERTY(EditAnywhere, Category = "GC")
	float Attack;

	UPROPERTY(EditAnywhere, Category = "GC")
	float Armor;

	UPROPERTY(EditAnywhere, Category = "GC")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "GC")
	float AttackSpeed;

	FCharacterBaseStats() :
		MaxHp(1.f),
		Attack(0.f),
		Armor(0.f),
		MoveSpeed(1.f),
		AttackSpeed(1.f) {}

	FCharacterBaseStats
	(float InMaxHp, float InAttack, float InArmor, float InMoveSpeed, float InAttackSpeed) :
		MaxHp(InMaxHp),
		Attack(InAttack),
		Armor(InArmor),
		MoveSpeed(InMoveSpeed),
		AttackSpeed(InAttackSpeed) {}
};

USTRUCT(BlueprintType)
struct FStatModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "GC")
	ECharacterStatType StatType = ECharacterStatType::None;

	UPROPERTY(EditAnywhere, Category = "GC")
	EStatModifierOp Operation = EStatModifierOp::None;

	UPROPERTY(EditAnywhere, Category = "GC")
	float Value = 0.f;

	UPROPERTY(EditAnywhere, Category = "GC")
	float Duration = 0.f;

	UPROPERTY(EditAnywhere, Category = "GC")
	float RemainTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "GC")
	FName SourceTag;
};
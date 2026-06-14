#pragma once

#include "CoreMinimal.h"

#include "Game\GC_Grid.h"
#include "Game\GC_GridManager.h"

#include "GC_Skill.generated.h"

UENUM(BlueprintType)
enum class EEffectTarget : uint8
{
	Opponent,
	Ally,
};

UENUM(BlueprintType)
enum class EScalingStat : uint8
{
	Flat,
	Attack,
	MaxHp
};

USTRUCT(BlueprintType)
struct FSkillContext
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 SkillInstanceID;

	UPROPERTY()
	int32 EffectIndex;

	UPROPERTY()
	TArray<FGridCoord> EffectArea;

	UPROPERTY()
	TObjectPtr<AGC_GridManager> GridManager;

	UPROPERTY()
	TObjectPtr<AGC_Character> User;

	UPROPERTY()
	TObjectPtr<AGC_Character> Target;
};
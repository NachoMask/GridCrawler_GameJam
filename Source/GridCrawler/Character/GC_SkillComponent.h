// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GC_SkillComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillChangeDelegate, int32 /*SkillIndex*/, UTexture2D* /*SkillIcon*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillInUseDelegate, int32 /*SkillIndex*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillCooldownDelegate, int32 /*SkillIndex*/, float /*Cooldown*/);

class UGC_SkillData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDCRAWLER_API UGC_SkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGC_SkillComponent();

public:
	FOnSkillChangeDelegate OnSkillChange;
	FOnSkillInUseDelegate OnSkillInUse;
	FOnSkillCooldownDelegate OnSkillCooldown;

	void InitSkills(const TArray<const UGC_SkillData*>& InSkills);

	void ChangeSkill(int32 SkillIndex, const UGC_SkillData* InSkill);

	bool IsSkillInCooldown(int32 Idx, float Time);

	void BroadcastCurrentSkills();

	void BroadcastCurrentSkills(float ChangedAttack);

public:
	int32 GetMaxSkillNum() const { return MaxSkillNum; }

	int32 GetNextEmptySkillSlot() const;

	const TArray<const UGC_SkillData*>& GetAllSkillData() const { return Skills; }

	const UGC_SkillData* GetSkillData(int32 Idx) const;

	void SetSkillInUse(int32 Idx);

	float GetSkillExecutedTime(int32 Idx) const;

	void SetSkillExecutedTime(int32 Idx, float Time);

protected:
	UPROPERTY(VisibleAnywhere, Category = "GC|Skill")
	int32 MaxSkillNum = 4;

	UPROPERTY(VisibleAnywhere, Category = "GC|Skill")
	TArray<const UGC_SkillData*> Skills;

	UPROPERTY(VisibleAnywhere, Category = "GC|Skill")
	TArray<float> SkillExecutedTime;
};

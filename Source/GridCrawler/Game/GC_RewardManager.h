// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Character\GC_CharacterStat.h"
#include "Skill\GC_SkillData.h"

#include "GC_RewardManager.generated.h"

class AGC_CharacterPlayer;
class AGC_PlayerController;

DECLARE_MULTICAST_DELEGATE_FourParams(FOnSkillRewardDelegate,
	UTexture2D* /*InSkillIcon*/, FName /*InSkillName*/, float /*InSkillCooldown*/, FName /*InSkillDesc*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatRewardDelegate,
	int32 /*StatEnumIndex*/, float /*InStatValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRewardOutCursorDelegate,
	int32 /*CursorIndex*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRewardInCursorDelegate,
	int32 /*CursorIndex*/);
DECLARE_MULTICAST_DELEGATE(FOnRewardRemoveDelegate);

UENUM(BlueprintType)
enum class ERewardType : uint8
{
	Skill,
	Stat,
	Heal
};

USTRUCT(BlueprintType)
struct FRewardData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERewardType RewardType;

	// In Case of Skill Reward
	UPROPERTY(EditAnywhere)
	UGC_SkillData* SkillData;

	// In Case of Stat Reward
	UPROPERTY(EditAnywhere)
	ECharacterStatType StatType;

	UPROPERTY(EditAnywhere)
	EStatModifierOp Op;

	UPROPERTY(EditAnywhere)
	float Value;
};

UCLASS()
class GRIDCRAWLER_API AGC_RewardManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGC_RewardManager();

public:
	FOnSkillRewardDelegate OnSkillReward;
	FOnStatRewardDelegate OnStatReward;
	FOnRewardOutCursorDelegate OnRewardOutCursor;
	FOnRewardInCursorDelegate OnRewardInCursor;
	FOnRewardRemoveDelegate OnRewardRemove;

	void Init(AGC_CharacterPlayer* InPlayerCharacter);
	void RegisterPlayerController(AGC_PlayerController* InPlayerController);

	/** Start Reward */
	void GenerateStartSkill(int32 Count);

	/** Reward */
	void GenerateReward(int32 Count);
	void RemoveReward();

	void SelectReward();

	/** Control */
	void MoveRewardCursor(float AxisValue);

protected:
	UPROPERTY(VisibleAnywhere, Category = "GC|Cahced")
	TObjectPtr<AGC_PlayerController> PlayerController = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Cached")
	TObjectPtr<AGC_CharacterPlayer> PlayerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Reward")
	TArray<FRewardData> StatRewardPool;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Reward")
	TArray<FRewardData> SkillRewardPool;

	UPROPERTY(VisibleAnywhere, Category = "GC|Reward")
	TArray<FRewardData> CurrentReward;

	UPROPERTY(VisibleAnywhere, Category = "GC|Reward")
	TArray<FName> SelectedRewardID;

	UPROPERTY(VisibleAnywhere, Category = "GC|Reward")
	int32 RewardCursor = -1;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GC_Character.h"

#include "Character\GC_CharacterStat.h"

#include "GC_CharacterPlayer.generated.h"

class AGC_PlayerController;
class UGC_CharacterStat;
class UGC_SkillData;
class UWidgetComponent;
class UGC_HpTextWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerMoveDelegate, const FGridCoord& /*Coord*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelUpDelegate, int32 /*Level*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnExpChangeDelegate, int32 /*CurrentExp*/, int32 /*MaxExp*/);

UCLASS()
class GRIDCRAWLER_API AGC_CharacterPlayer : public AGC_Character
{
	GENERATED_BODY()
	
public:
	AGC_CharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	FOnPlayerMoveDelegate OnPlayerMove;
	FOnLevelUpDelegate OnLevelUp;
	FOnExpChangeDelegate OnExpChange;

	/** Player Spawn Function */
	virtual void Init(AGC_GridManager* InGridManager, AGC_BattleManager* InBattleManager, const FGridCoord& Coord) override;

	/** Player Input Function : Move */
	void MoveAction_Started(FVector2D Value);
	void MoveAction_Triggered(FVector2D Value);
	void MoveAction_Completed(FVector2D Value);

	/** Player Input Function : Attack */
	void SkillAction_Started(int32 Idx);
	void SkillAction_Triggered(int32 Idx);
	void SkillAction_Completed(int32 Idx);

	UGC_SkillComponent* GetSkillComponent() const
	{
		return SkillComp;
	}

	int32 GetEmptySkillSlot() const;

	void ChangeSkill(const UGC_SkillData* Skill);

	void RequestBroadcastStats();
	void RequestBroadcastSkills();

	/** Level */
	void AddExp(int32 Amount);
	void CheckExp();
	int32 GetMaxExp() const
	{
		return MaxExp;
	}

protected:
	virtual void TryMove(ECharacterDirection InDirection) override;

	virtual void EndSkill(UAnimMontage* AnimMontage, bool bInterrupted) override;

	virtual void TryForcedMove() override;

	/** Function of InputValue(FVector2D) -> Direction(ECharacterDirection) */
	ECharacterDirection InputValueToDirection(FVector2D Value);

	/** Level */
	void LevelUp();

	/** Death */
	virtual void DeathCharacter() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AGC_PlayerController> PlayerController;
	
	UPROPERTY(VisibleAnywhere, Category = "GC|Stat")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, Category = "GC|Stat")
	int32 CurrentExp = 0;

	UPROPERTY(VisibleAnywhere, Category = "GC|Stat")
	int32 MaxExp = 3;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Stat")
	FCharacterBaseStats PlayerStartStats;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Skill")
	TArray<const UGC_SkillData*> StartSkills;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Skill")
	TObjectPtr<UGC_SkillData> StartDashSkill;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> DeathMontage = nullptr;

protected:
	/** Widget*/
	UPROPERTY(VisibleAnywhere, Category = "GC|Widget")
	TObjectPtr<UWidgetComponent> HpText;

	UPROPERTY(EditAnywhere, Category = "GC|Widget")
	TSubclassOf<UGC_HpTextWidget> HpTextClass = nullptr;
};

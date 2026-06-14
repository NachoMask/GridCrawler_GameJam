// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GC_Character.h"

#include "Game\GC_Grid.h"

#include "GC_CharacterEnemy.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathDelegate, AGC_CharacterEnemy* /*Enemy*/)

class UGC_EnemyData;
class UGC_SkillData;
class UWidgetComponent;
class UGC_HpTextWidget;
class AGC_ExpTextActor;

UCLASS()
class GRIDCRAWLER_API AGC_CharacterEnemy : public AGC_Character
{
	GENERATED_BODY()
	
public:
	AGC_CharacterEnemy();

protected:
	virtual void Tick(float DeltaSeconds) override;

public:
	/** Enemy Death Delegate */
	FOnEnemyDeathDelegate OnEnemyDeath;
	
	virtual void Init(AGC_GridManager* InGridManager, AGC_BattleManager* InBattleManager, const FGridCoord& Coord) override;
	void ApplyEnemyData(UGC_EnemyData* InData);

	/** Move Related Function */
	FGridCoord GetPrevCoord() const
	{
		return PrevCoord;
	}

	int32 GetEnemyExp() const;

	void RequestMove(ECharacterDirection MoveDir);

protected:
	virtual void TryMove(ECharacterDirection InDirection) override;

	virtual void EndSkill(UAnimMontage* AnimMontage, bool bInterrupted) override;

	virtual void DeathCharacter() override;

	void DeathProcess(UAnimMontage* AnimMontage, bool bInterrupted);

protected:
	/** Data */
	UPROPERTY(VisibleAnywhere, Category = "GC|Data")
	TObjectPtr<UGC_EnemyData> Data;

	UPROPERTY(VisibleAnywhere, Category = "GC|EnemyAction")
	float DecisionTimer = 0.f;

	UPROPERTY(VisibleAnywhere)
	FGridCoord PrevCoord = FGridCoord(-1, -1);

	bool bIsDead = false;

	/** Widget*/
	UPROPERTY(VisibleAnywhere, Category = "GC|Widget")
	TObjectPtr<UWidgetComponent> HpText;

	UPROPERTY(EditAnywhere, Category = "GC|Widget")
	TSubclassOf<UGC_HpTextWidget> HpTextClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "GC|Widget")
	TSubclassOf<AGC_ExpTextActor> ExpTextActorClass = nullptr;
};

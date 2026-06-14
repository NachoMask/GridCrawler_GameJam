// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GC_StageManager.generated.h"

class AGC_GridManager;
class AGC_SpawnManager;
class AGC_RewardManager;
class AGC_PlayerController;
class AGC_CharacterEnemy;
class UGC_EnemyData;

USTRUCT()
struct FStageSpawnSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<UGC_EnemyData*> EnemyPool;

	UPROPERTY(EditAnywhere)
	int32 SpawnCount;
};

UCLASS()
class GRIDCRAWLER_API AGC_StageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGC_StageManager();

public:
	void Init(AGC_GridManager* InGridManager, AGC_SpawnManager* InSpawnManager, AGC_RewardManager* InRewardManager);
	void RegisterPlayerController(AGC_PlayerController* InPlayerController);

	void StartStage();
	void EndStage();

	void StartNextStage();

protected:
	void BindEnemy(AGC_CharacterEnemy* Enemy);

	void OnEnemyDeath(AGC_CharacterEnemy* Enemy);

protected:
	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_GridManager> GridManager = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_SpawnManager> SpawnManager = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_RewardManager> RewardManager = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Controller")
	TObjectPtr<AGC_PlayerController> PlayerController = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Stage")
	int32 CurrentStageLevel = -1;
	
	UPROPERTY(VisibleAnywhere, Category = "GC|Stage")
	int32 TutorialStageLevel = 0;

	UPROPERTY(VisibleAnywhere, Category = "GC|Stage")
	int32 AliveEnemyCount = 0;

	UPROPERTY(EditAnywhere, Category = "GC|Stage")
	TArray<FStageSpawnSetting> Stages;
};

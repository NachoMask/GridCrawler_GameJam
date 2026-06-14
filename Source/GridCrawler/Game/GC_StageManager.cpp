// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_StageManager.h"

#include "Game\GC_GridManager.h"
#include "Game\GC_SpawnManager.h"
#include "Game\GC_RewardManager.h"
#include "Player\GC_PlayerController.h"
#include "Character\GC_CharacterEnemy.h"
#include "Enemy\GC_EnemyData.h"
#include "Algo/RandomShuffle.h"

AGC_StageManager::AGC_StageManager()
{

}

void AGC_StageManager::Init(AGC_GridManager* InGridManager, AGC_SpawnManager* InSpawnManager, AGC_RewardManager* InRewardManager)
{
	GridManager = InGridManager;
	SpawnManager = InSpawnManager;
	RewardManager = InRewardManager;
}

void AGC_StageManager::RegisterPlayerController(AGC_PlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}

void AGC_StageManager::StartStage()
{
	UE_LOG(LogTemp, Warning, TEXT("Stage Start"));

	if (CurrentStageLevel >= Stages.Num()) return;

	TArray<FGridCoord> EmptyTiles = GridManager->FilterEmptyCoords(GridManager->GetAllCoords());
	Algo::RandomShuffle(EmptyTiles);

	// Get Stage Setting According to CurrentStageLevel
	const FStageSpawnSetting& Setting = Stages[CurrentStageLevel];

	// Set AliveEnemyCount According to StageSpawnSetting
	AliveEnemyCount = FMath::Min(Setting.SpawnCount, EmptyTiles.Num());
	
	// Check EnemyPoolCount is Greater than 0
	int32 EnemyPoolCount = Setting.EnemyPool.Num();
	check(EnemyPoolCount > 0);

	// Request EnemySpawn According to AliveEnemyCount
	for (int i = 0; i < AliveEnemyCount; ++i)
	{
		// Choose Random EnemyData In EnemyPool
		int32 Index = FMath::RandRange(0, EnemyPoolCount - 1);
		UGC_EnemyData* EnemyData = Setting.EnemyPool[Index];

		// Reqeust Enemy Spawn
		AGC_CharacterEnemy* Enemy
			= SpawnManager->RequestSpawnProcess(FSpawnRequest(EnemyData, EmptyTiles));

		// Bind Enemy's Death Delegate
		BindEnemy(Enemy);
	}
}

void AGC_StageManager::EndStage()
{
	if (RewardManager)
	{
		if (CurrentStageLevel == TutorialStageLevel)
		{

		}

		RewardManager->GenerateReward(3);
	}
}

void AGC_StageManager::StartNextStage()
{
	// Increase Stage Level
	++CurrentStageLevel;

	// If Next Stage Level is Valid
	if (Stages.IsValidIndex(CurrentStageLevel))
	{
		// Start Stage
		StartStage();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Clear"));
	}
}

void AGC_StageManager::BindEnemy(AGC_CharacterEnemy* Enemy)
{
	// Bind Enemy's Death Delegate 
	Enemy->OnEnemyDeath.AddUObject(this, &AGC_StageManager::OnEnemyDeath);
}

void AGC_StageManager::OnEnemyDeath(AGC_CharacterEnemy* Enemy)
{
	// Decrease AliveEnemyCount
	--AliveEnemyCount;
	UE_LOG(LogTemp, Log, TEXT("Enemy Died"));

	// If Alive Enemy Count is 0
	if (AliveEnemyCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stage %d Clear"), CurrentStageLevel + 1);

		EndStage();
	}
}

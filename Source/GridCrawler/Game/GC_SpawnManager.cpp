// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_SpawnManager.h"

#include "Game\GC_GridManager.h"
#include "Game\GC_BattleManager.h"
#include "Character\GC_CharacterEnemy.h"
#include "Enemy\GC_EnemyData.h"
#include "Algo/RandomShuffle.h"

AGC_SpawnManager::AGC_SpawnManager()
{

}

void AGC_SpawnManager::Init(AGC_GridManager* InGridManager, AGC_BattleManager* InBattleManager)
{
	GridManager = InGridManager;
	BattleManager = InBattleManager;
}

AGC_CharacterEnemy* AGC_SpawnManager::RequestSpawnProcess(FSpawnRequest Request)
{
	// Check GridManager Validity
	check(GridManager);

	// Filter Empty Coords from Requested Coords And Shuffle
	TArray<FGridCoord> EmptyTiles = GridManager->FilterEmptyCoords(Request.CandidateCoords);
	// Algo::RandomShuffle(EmptyTiles);

	// Spawn Enemy From Shuffled Empty Coords In Loop
	AGC_CharacterEnemy* Enemy = nullptr;
	for (FGridCoord EmptyTile : EmptyTiles)
	{
		Enemy = SpawnEnemy(EmptyTile, Request.EnemyData);

		// If Enemy Spawned, End Loop
		if (Enemy != nullptr) break;
	}

	if (Enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Spawn Failed!"));
	}

	return Enemy;
}

AGC_CharacterEnemy* AGC_SpawnManager::SpawnEnemy(const FGridCoord& InCoord, UGC_EnemyData* EnemyData)
{
	// Check GridManager Validity
	check(GridManager);

	// Spawn Enemy to the EnemySpawnLocation
	AGC_CharacterEnemy* Enemy = GetWorld()->SpawnActor<AGC_CharacterEnemy>(EnemyData->EnemyClass, FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator);

	// Apply Enemy Data
	Enemy->ApplyEnemyData(EnemyData);

	// Enemy Init
	Enemy->Init(GridManager, BattleManager, InCoord);

	return Enemy;
}

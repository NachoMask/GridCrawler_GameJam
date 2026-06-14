// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Game\GC_Grid.h"

#include "GC_SpawnManager.generated.h"

class AGC_GridManager;
class AGC_BattleManager;
class AGC_CharacterEnemy;
class UGC_EnemyData;

USTRUCT(BlueprintType)
struct FSpawnRequest
{
	GENERATED_BODY()

	TObjectPtr<UGC_EnemyData> EnemyData = nullptr;

	TArray<FGridCoord> CandidateCoords;
	
	FSpawnRequest() {}
	FSpawnRequest(UGC_EnemyData* InData, const TArray<FGridCoord>& InCoords)
		: EnemyData(InData), CandidateCoords(InCoords) {}
};


UCLASS()
class GRIDCRAWLER_API AGC_SpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGC_SpawnManager();

public:
	void Init(AGC_GridManager* InGridManager, AGC_BattleManager* InBattleManager);
	
	AGC_CharacterEnemy* RequestSpawnProcess(FSpawnRequest Request);

protected:
	AGC_CharacterEnemy* SpawnEnemy(const FGridCoord& InCoord, UGC_EnemyData* EnemyData);

protected:
	TObjectPtr<AGC_GridManager> GridManager = nullptr;
	TObjectPtr<AGC_BattleManager> BattleManager = nullptr;
};

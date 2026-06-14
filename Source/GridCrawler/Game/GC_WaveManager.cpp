// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_WaveManager.h"

#include "Game\GC_GridManager.h"
#include "Game\GC_SpawnManager.h"
#include "Character\GC_CharacterPlayer.h"
#include "Character\GC_CharacterEnemy.h"
#include "Algo/RandomShuffle.h"

AGC_WaveManager::AGC_WaveManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGC_WaveManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bTimerTick)
	{
		int32 CurrentTime = FMath::FloorToInt(GetWorld()->GetTimeSeconds() - StartTime);
		if (WaveTime != CurrentTime)
		{
			WaveTime = CurrentTime;
			OnTimeChange.Broadcast(WaveTime);

			if (Waves.IsValidIndex(NextWaveLevel) &&
				WaveTime >= Waves[NextWaveLevel].WaveTime)
			{
				SpawnWave();
			}
		}
	}
}

void AGC_WaveManager::Init(AGC_GridManager* InGridManager, AGC_SpawnManager* InSpawnManager, AGC_CharacterPlayer* InCharacterPlayer)
{
	GridManager = InGridManager;
	SpawnManager = InSpawnManager;
	CharacterPlayer = InCharacterPlayer;

	/** MsCheck*/
	/*TArray<UGC_EnemyData*> EnemyPool;
	for (int32 i = 0; i < SpawnCount; ++i)
	{
		EnemyPool.Add(MsCheckEnemy);
	}

	Waves.Add(FWaveSpawnSetting(EnemyPool));*/
}

void AGC_WaveManager::RequestWaveStart()
{
	StartTime = GetWorld()->GetTimeSeconds();
	WaveTime = FMath::FloorToInt(StartTime);
	OnTimeChange.Broadcast(WaveTime);
	bTimerTick = true;
}

bool AGC_WaveManager::SpawnWave()
{
	if (NextWaveLevel >= Waves.Num()) return false;

	UE_LOG(LogTemp, Log, TEXT("Wave %d Start"), NextWaveLevel);

	TArray<FGridCoord> AllTiles = GridManager->GetAllCoords();
	Algo::RandomShuffle(AllTiles);
	/*FRandomStream RandomSeed(1999521);
	ShuffleSpawnCoord(AllTiles, RandomSeed);*/

	const FWaveSpawnSetting& Setting = Waves[NextWaveLevel];

	for (UGC_EnemyData* Enemy : Setting.EnemyPool)
	{
		AGC_CharacterEnemy* SpawnEnemy
			= SpawnManager->RequestSpawnProcess(FSpawnRequest(Enemy, AllTiles));

		BindEnemy(SpawnEnemy);
		++AliveEnemyCount;
	}

	++NextWaveLevel;
	return true;
}

void AGC_WaveManager::OnEnemyAllKill()
{
	// NextWaveStart
	if (SpawnWave() == false)
	{
		// Game Clear
		UE_LOG(LogTemp, Warning, TEXT("Game Clear"));

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
			[&]()
			{
				OnGameClear.Broadcast();
			}
		), 3.f, false);
	}
}

void AGC_WaveManager::BindEnemy(AGC_CharacterEnemy* Enemy)
{
	Enemy->OnEnemyDeath.AddUObject(this, &AGC_WaveManager::OnEnemyDeath);
}

void AGC_WaveManager::OnEnemyDeath(AGC_CharacterEnemy* Enemy)
{
	--AliveEnemyCount;

	if (CharacterPlayer)
	{
		// Player AddExp
		CharacterPlayer->AddExp(Enemy->GetEnemyExp());
	}

	if (AliveEnemyCount <= 0)
	{
		OnEnemyAllKill();
	}
}

//template<typename T>
//inline void AGC_WaveManager::ShuffleSpawnCoord(TArray<T>& Array, const FRandomStream Seed)
//{
//	for (int32 i = 0; i < Array.Num(); ++i)
//	{
//		const int32 SwapIndex = Seed.RandRange(0, i);
//
//		if (i != SwapIndex)
//		{
//			Array.Swap(i, SwapIndex);
//		}
//	}
//}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_GameMode.h"

#include "Game\GC_GridManager.h"
#include "Game\GC_WaveManager.h"
#include "Game\GC_StageManager.h"
#include "Game\GC_RewardManager.h"
#include "Game\GC_SpawnManager.h"
#include "Game\GC_BattleManager.h"
#include "Game\GC_IndicatorManager.h"
#include "Character\GC_CharacterPlayer.h"

void AGC_GameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World) return;

	// Spawn GridManager
	if (GridManagerClass)
	{
		GridManager = World->SpawnActor<AGC_GridManager>(GridManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	if (WaveManagerClass)
	{
		WaveManager = World->SpawnActor<AGC_WaveManager>(WaveManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	if (RewardManagerClass)
	{
		RewardManager = World->SpawnActor<AGC_RewardManager>(RewardManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	// Spawn SpawnManager
	if (SpawnManagerClass)
	{
		SpawnManager = World->SpawnActor<AGC_SpawnManager>(SpawnManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	// Spawn BattleManager
	if (BattleManagerClass)
	{
		BattleManager = World->SpawnActor<AGC_BattleManager>(BattleManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	if (IndicatorManagerClass)
	{
		IndicatorManager = World->SpawnActor<AGC_IndicatorManager>(IndicatorManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	check(GridManager);
	check(WaveManager);
	check(RewardManager);
	check(SpawnManager);
	check(BattleManager);
	check(IndicatorManager);

	// Spawn CharacterPlayer
	if (CharacterPlayerClass)
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC && GridManager)
		{
			// Spawn to PlayerSpawnCoord
			FVector SpawnLocation = GridManager->CoordToLocation(GridManager->GetPlayerSpawnCoord());
			FRotator SpawnRotation = FRotator::ZeroRotator;

			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			CharacterPlayer = World->SpawnActor<AGC_CharacterPlayer>(
				CharacterPlayerClass, SpawnLocation, SpawnRotation, Params);

			if (CharacterPlayer)
			{
				PC->Possess(CharacterPlayer);
			}
		}
	}

	// Switch to Use GridManager's Camera
	APlayerController* PC = World->GetFirstPlayerController();
	if (PC && GridManager)
	{
		PC->SetViewTargetWithBlend(GridManager);
	}

	// Initialize Other Managers to be Assigned GridManager
	if (SpawnManager && GridManager && BattleManager)
	{
		SpawnManager->Init(GridManager, BattleManager);
	}

	if (RewardManager  && CharacterPlayer)
	{
		RewardManager->Init(CharacterPlayer);
	}

	if (BattleManager && GridManager && IndicatorManager)
	{
		BattleManager->Init(GridManager, IndicatorManager);
	}

	if (IndicatorManager && GridManager)
	{
		IndicatorManager->Init(GridManager);
	}

	if (WaveManager && GridManager && SpawnManager && CharacterPlayer)
	{
		WaveManager->Init(GridManager, SpawnManager, CharacterPlayer);
	}

	if (CharacterPlayer && GridManager && BattleManager)
	{
		CharacterPlayer->Init(GridManager, BattleManager, GridManager->GetPlayerSpawnCoord());
		CharacterPlayer->OnPlayerMove.AddUObject(GridManager, &AGC_GridManager::RebuildPathfindField);
	}
}

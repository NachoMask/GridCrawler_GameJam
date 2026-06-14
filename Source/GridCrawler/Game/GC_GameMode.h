// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GC_GameMode.generated.h"

class AGC_GridManager;
class AGC_WaveManager;
class AGC_RewardManager;
class AGC_SpawnManager;
class AGC_BattleManager;
class AGC_IndicatorManager;
class AGC_CharacterPlayer;

UCLASS()
class GRIDCRAWLER_API AGC_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	AGC_WaveManager* GetWaveManager() const
	{
		return WaveManager;
	}

	AGC_RewardManager* GetRewardManager() const
	{
		return RewardManager;
	}

protected:
	UPROPERTY()
	TObjectPtr<AGC_GridManager> GridManager = nullptr;

	UPROPERTY()
	TObjectPtr<AGC_WaveManager> WaveManager = nullptr;

	UPROPERTY()
	TObjectPtr<AGC_RewardManager> RewardManager = nullptr;

	UPROPERTY()
	TObjectPtr<AGC_SpawnManager> SpawnManager = nullptr;

	UPROPERTY()
	TObjectPtr<AGC_BattleManager> BattleManager = nullptr;

	UPROPERTY()
	TObjectPtr<AGC_IndicatorManager> IndicatorManager = nullptr;

	UPROPERTY()
	TObjectPtr<AGC_CharacterPlayer> CharacterPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGC_GridManager> GridManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGC_WaveManager> WaveManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGC_RewardManager> RewardManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGC_SpawnManager> SpawnManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGC_BattleManager> BattleManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGC_IndicatorManager> IndicatorManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGC_CharacterPlayer> CharacterPlayerClass;
};

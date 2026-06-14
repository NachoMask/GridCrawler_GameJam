// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Game\GC_Grid.h"

#include "GC_WaveManager.generated.h"

class AGC_GridManager;
class AGC_SpawnManager;
class AGC_CharacterPlayer;
class AGC_CharacterEnemy;
class UGC_EnemyData;

class UArrayProperty;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChangeDelegate, int32 /*Time*/);
DECLARE_MULTICAST_DELEGATE(FOnGameClearDelegate);

USTRUCT(BlueprintType)
struct FWaveSpawnSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 WaveTime = 0;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UGC_EnemyData>> EnemyPool;

	UPROPERTY(EditAnywhere)
	float SpawnInterval = 0.f;

	FWaveSpawnSetting() {}
	FWaveSpawnSetting(const TArray<UGC_EnemyData*> InEnemyPool)
		: WaveTime(3), EnemyPool(InEnemyPool)
	{}
};

UCLASS()
class GRIDCRAWLER_API AGC_WaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGC_WaveManager();

protected:
	virtual void Tick(float DeltaSeconds) override;

public:
	FOnTimeChangeDelegate OnTimeChange;
	FOnGameClearDelegate OnGameClear;

	void Init(AGC_GridManager* InGridManager, AGC_SpawnManager* InSpawnManager, AGC_CharacterPlayer* InCharacterPlayer);
	void RequestWaveStart();
	bool SpawnWave();

protected:
	void OnEnemyAllKill();

	void BindEnemy(AGC_CharacterEnemy* Enemy);
	void OnEnemyDeath(AGC_CharacterEnemy* Enemy);

protected:
	/** Manager */
	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_GridManager> GridManager;

	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_SpawnManager> SpawnManager;

	UPROPERTY(VisibleAnywhere, Category = "GC|Player")
	TObjectPtr<AGC_CharacterPlayer> CharacterPlayer;

	/** Wave */
	UPROPERTY(VisibleAnywhere, Category = "GC|Wave")
	float StartTime = -1.f;

	UPROPERTY(VisibleAnywhere, Category = "GC|Wave")
	int32 WaveTime;

	bool bTimerTick = false;

	UPROPERTY(VisibleAnywhere, Category = "GC|Wave")
	int32 NextWaveLevel = 0;

	UPROPERTY(VisibleAnywhere, Category = "GC|Wave")
	int32 AliveEnemyCount = 0;

	UPROPERTY(EditAnywhere, Category = "GC|Wave")
	TArray<FWaveSpawnSetting> Waves;

	/** ms Check */
	/*UPROPERTY(EditDefaultsOnly, Category = "GC|MsCheck")
	TObjectPtr<UGC_EnemyData> MsCheckEnemy;
	UPROPERTY(EditDefaultsOnly, Category = "GC|MsCheck")
	int32 SpawnCount = 0;

	template<typename T>
	void ShuffleSpawnCoord(TArray<T>& Array, const FRandomStream Seed);*/
};
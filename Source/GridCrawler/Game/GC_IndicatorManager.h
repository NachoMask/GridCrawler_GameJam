// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Game\GC_Grid.h"

#include "GC_IndicatorManager.generated.h"

class AGC_GridManager;
class AGC_IndicatorActor;

UCLASS()
class GRIDCRAWLER_API AGC_IndicatorManager : public AActor
{
	GENERATED_BODY()
	
public:
	AGC_IndicatorManager();
	
public:
	void Init(AGC_GridManager* InGridManager);

	void CreatePlayerIndicator(FIntPoint Key, const TArray<FGridCoord> Indicator);

	void CreateEnemyIndicator(FIntPoint Key, const TArray<FGridCoord> Indicator);

	void RemoveIndicator(FIntPoint Key);

protected:
	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_GridManager> GridManager = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Indicator")
	TSubclassOf<AGC_IndicatorActor> PlayerIndicatorClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Indicator")
	TSubclassOf<AGC_IndicatorActor> EnemyIndicatorClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Indicator")
	TMap<FIntPoint, AGC_IndicatorActor*> ActiveIndicators;
};

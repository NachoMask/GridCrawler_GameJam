// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_IndicatorManager.h"

#include "Game\GC_GridManager.h"
#include "Game\GC_IndicatorActor.h"

AGC_IndicatorManager::AGC_IndicatorManager()
{

}

void AGC_IndicatorManager::Init(AGC_GridManager* InGridManager)
{
	GridManager = InGridManager;
}

void AGC_IndicatorManager::CreatePlayerIndicator(FIntPoint Key, const TArray<FGridCoord> Indicator)
{
	check(PlayerIndicatorClass);

	// UE_LOG(LogTemp, Log, TEXT("Create Indicator"));

	AGC_IndicatorActor* IndicatorActor
		= GetWorld()->SpawnActor<AGC_IndicatorActor>(PlayerIndicatorClass);

	IndicatorActor->Init(GridManager, Indicator);

	ActiveIndicators.Add(Key, IndicatorActor);
}

void AGC_IndicatorManager::CreateEnemyIndicator(FIntPoint Key, const TArray<FGridCoord> Indicator)
{
	check(EnemyIndicatorClass);

	// UE_LOG(LogTemp, Log, TEXT("Create Indicator"));

	AGC_IndicatorActor* IndicatorActor
		= GetWorld()->SpawnActor<AGC_IndicatorActor>(EnemyIndicatorClass);

	IndicatorActor->Init(GridManager, Indicator);

	ActiveIndicators.Add(Key, IndicatorActor);
}

void AGC_IndicatorManager::RemoveIndicator(FIntPoint Key)
{
	if (ActiveIndicators.Contains(Key))
	{
		ActiveIndicators[Key]->Destroy();

		ActiveIndicators.Remove(Key);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO ActiveSkill's Key"));
	}
}

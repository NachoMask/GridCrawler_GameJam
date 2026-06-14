// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Game\GC_Grid.h"

#include "GC_IndicatorActor.generated.h"

class AGC_GridManager;

UCLASS()
class GRIDCRAWLER_API AGC_IndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGC_IndicatorActor();

public:
	void Init(AGC_GridManager* GridManager, const TArray<FGridCoord>& Indicator);

protected:
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Indicator")
	TObjectPtr<UInstancedStaticMeshComponent> ISM;
};

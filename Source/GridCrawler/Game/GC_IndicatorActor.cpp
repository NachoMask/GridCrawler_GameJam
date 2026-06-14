// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_IndicatorActor.h"

#include "Game\GC_GridManager.h"
#include "Components\InstancedStaticMeshComponent.h"

// Sets default values
AGC_IndicatorActor::AGC_IndicatorActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SetRootComponent(Root);

	ISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM"));
	ISM->SetupAttachment(Root);
}

void AGC_IndicatorActor::Init(AGC_GridManager* GridManager, const TArray<FGridCoord>& Indicator)
{
	// Set ISM'S Aplha
	ISM->NumCustomDataFloats = 1;
	
	float GridSizeX = GridManager->GetGridSizeX();
	float GridSizeY = GridManager->GetGridSizeY();
	float WorldGridSizeX = GridManager->GetWorldGridSizeX();
	float WorldGridSizeY = GridManager->GetWorldGridSizeY();

	for (const FGridCoord& Coord : Indicator)
	{
		FTransform Transform;
		Transform.SetLocation(GridManager->CoordToLocation(Coord, 1.f));
		Transform.SetScale3D(FVector(
			(GridSizeY / WorldGridSizeY) * 0.75f,
			(GridSizeX / WorldGridSizeX) * 0.75f,
			1.f));

		int32 Index = ISM->AddInstance(Transform);
		// ISM->SetCustomDataValue(Index, 0, 0.5f, true);
	}
}
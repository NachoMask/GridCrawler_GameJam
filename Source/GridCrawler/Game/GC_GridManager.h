// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Game\GC_Grid.h"

#include "GC_GridManager.generated.h"

class UCameraComponent;
class AGC_Character;

UCLASS()
class GRIDCRAWLER_API AGC_GridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGC_GridManager();

protected:
	virtual void BeginPlay() override;

public:
	FGridCoord GetPlayerSpawnCoord() const;

	/** Whether Coord's Tile is Within Boundaries */
	bool IsInGridBounds(const FGridCoord& Coord) const;

	ECharacterDirection CoordToDirection(const FGridCoord& Coord) const;

	/** Rotator -> Direction */
	ECharacterDirection RotatorToDirection(const FRotator& InRotator);

	/** Direction -> Rotator */
	FRotator DirectionToRotator(ECharacterDirection InDirection);

	/** Function of Coord(FGridCoord) -> Location(FVector) */
	FVector CoordToLocation(const FGridCoord& Coord) const;
	FVector CoordToLocation(const FGridCoord& Coord, float Z) const;

	/** Function of Location(FVector) -> Coord(FGridCoord) */
	FGridCoord LocationToCoord(const FVector& Location) const;

	/** Function of Coord(FGridCoord) -> TileIndex(int32) */
	int32 CoordToGridTileIndex(const FGridCoord& Coord) const;

	/** Function of TileIndex(int32) -> Coord(FGridCoord) */
	FGridCoord GridTileIndexToCoord(int32 Index) const;

	bool IsWalkableTile(const FGridCoord& Coord) const;

	bool IsOccupiedTile(const FGridCoord& Coord) const;

	bool IsCanEnterTile(const FGridCoord& Coord) const;

	AActor* GetOccupyingActor(const FGridCoord& Coord) const;

	void SetOccupyingActor(const FGridCoord& Coord, AActor* NewActor);

	void ClearOccupyingActor(const FGridCoord& Coord);

	int32 GetDistCoordToCoord(const FGridCoord& CoordA, const FGridCoord& CoordB);

	int32 GetTileCost(const FGridCoord& Coord) const;

	int32 GetDistToPlayer(const FGridCoord& Coord) const;

	void SetDistToPlayer(const FGridCoord& Coord, int32 Dist);

	ECharacterDirection GetDirToPlayer(const FGridCoord& Coord) const;

	void SetDirToPlayer(const FGridCoord& Coord, ECharacterDirection Dir);

	ECharacterDirection PathfindToPlayer(const FGridCoord& Start, const FGridCoord& PrevCoord);

	TArray<FGridCoord> GetAllCoords();

	TArray<FGridCoord> FilterEmptyCoords(const TArray<FGridCoord>& Coords);

	FGridCoord RotateCoordOffset(const FGridCoord& Coord, ECharacterDirection Dir);

	/** Getter For Neighbor Tile's Array of Coord's Tile*/
	TArray<FGridCoord> GetNeighborCoords(const FGridCoord& Coord) const;

	UFUNCTION()
	void RebuildPathfindField(const FGridCoord& Target);

public:
	float GetGridSizeX() const { return GridSizeX; }

	float GetGridSizeY() const { return GridSizeY; }

	float GetWorldGridSizeX() const { return WorldGridSizeX; }
	float GetWorldGridSizeY() const { return WorldGridSizeY; }

protected:
	/** Generate Grid Tile According To Grid Setting */
	void GenerateGridTile();

	void SetCameraLocation();

	/** Pathfind Related Fields */
	/** Build Distance(to Player) Field*/
	void BuildDistanceField(const FGridCoord& Target);
	
	/** Reset Distance(to Player) Field*/
	void ResetDistanceField();

	void BuildDirectionField();

protected:
	/** Camera */
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Camera")
	TObjectPtr<UCameraComponent> Camera = nullptr;

	/** Grid */
	UPROPERTY(EditAnywhere, Category = "GC|Grid")
	int32 GridWidth;

	UPROPERTY(EditAnywhere, Category = "GC|Grid")
	int32 GridHeight;

	UPROPERTY(EditAnywhere, Category = "GC|Grid")
	float GridSizeX = 100.f;

	UPROPERTY(EditAnywhere, Category = "GC|Grid")
	float GridSizeY = 100.f;

	UPROPERTY(EditAnywhere, Category = "GC|Grid")
	float GridFieldZ = 92.f;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Grid")
	FVector GridOrigin;

	float WorldGridSizeX = 200.f;
	float WorldGridSizeY = 200.f;
	
	// BattleField's ISM(InstancedStaticMesh) 
	UPROPERTY(VisibleAnywhere, Category = "GC|Grid")
	TObjectPtr<UInstancedStaticMeshComponent> Field;

	/*UPROPERTY(VisibleAnywhere, Category = "GC|Grid")
	TObjectPtr<UInstancedStaticMeshComponent> OccupiedField;*/
	
	UPROPERTY(VisibleAnywhere, Category = "GC|Grid")
	TArray<FGridTileData> GridTileDataArr;



	/** ms Check */
	/*UPROPERTY(EditDefaultsOnly, Category = "GC|msCheck")
	int32 CheckCaseNum = 0;

	int32 CurrentCaseNum = 0;

	float AstarSum = 0.f;
	int32 AstarCount = 0.f;

	float AllSum = 0.f;*/
};

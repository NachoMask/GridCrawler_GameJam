#pragma once

#include "CoreMinimal.h"

#include "Math/IntPoint.h"

#include "GC_Grid.generated.h"

#define INVALID_COORD FGridCoord{-1, -1}

UENUM(BlueprintType)
enum class ECharacterDirection : uint8
{
	None,
	Up,
	Down,
	Left,
	Right
};

USTRUCT(BlueprintType)
struct FGridCoord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Point;

	FGridCoord() : Point(0, 0) {}
	FGridCoord(int32 X, int32 Y) : Point(X, Y) {}

	// Operator Overloading : Coord == Coord
	bool operator == (const FGridCoord& Other) const
	{
		return Point == Other.Point;
	}

	// Operator Overloading : Coord != Coord
	bool operator != (const FGridCoord& Other) const
	{
		return !(*this == Other);
	}

	// Operator Overloading : Coord + Coord
	FGridCoord operator + (const FGridCoord& Other) const
	{
		return FGridCoord(Point.X + Other.Point.X, Point.Y + Other.Point.Y);
	}

	// Operator Overloading : Coord + Dir
	FGridCoord operator + (ECharacterDirection Dir) const
	{
		switch (Dir)
		{
		case ECharacterDirection::Right:
			return FGridCoord(Point.X + 1, Point.Y);
		case ECharacterDirection::Left:
			return FGridCoord(Point.X - 1, Point.Y);
		case ECharacterDirection::Down:
			return FGridCoord(Point.X, Point.Y - 1);
		case ECharacterDirection::Up:
			return FGridCoord(Point.X, Point.Y + 1);
		}

		return FGridCoord(Point.X, Point.Y);
	}

	// Operator Overloading : Coord - Coord
	FGridCoord operator - (const FGridCoord& Other) const
	{
		// return FGridCoord(X - Other.X, Y - Other.Y);
		return FGridCoord(Point.X - Other.Point.X, Point.Y - Other.Point.Y);
	}

	// Operator Overloading : Coord += Coord
	FGridCoord& operator += (const FGridCoord& Other)
	{
		Point.X += Other.Point.X;
		Point.Y += Other.Point.Y;

		return *this;
	}

	// Operator Overloading : Coord += Dir
	FGridCoord& operator += (ECharacterDirection Dir)
	{
		switch (Dir)
		{
		case ECharacterDirection::Up:
			Point.Y += 1;
			break;
		case ECharacterDirection::Down:
			Point.Y -= 1;
			break;
		case ECharacterDirection::Left:
			Point.X -= 1;
			break;
		case ECharacterDirection::Right:
			Point.X += 1;
			break;
		}

		return *this;
	}

	// Operator Overloading : Coord -= Coord
	FGridCoord& operator -= (const FGridCoord& Other)
	{
		Point.X -= Other.Point.X;
		Point.Y -= Other.Point.Y;

		return *this;
	}
};

FORCEINLINE uint32 GetTypeHash(const FGridCoord& Value)
{
	return HashCombine(::GetTypeHash(Value.Point.X), ::GetTypeHash(Value.Point.Y));
}

USTRUCT(BlueprintType)
struct FGridTileData
{
	GENERATED_BODY()

	// This Tile's Coord
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridCoord GridCoord;

	// Whether This Tile Is Walkable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWalkable = true;

	// Actor Occupying This Tile
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor> OccupyingActor = nullptr;

	// Cost of Moving
	UPROPERTY(VisibleAnywhere)
	int32 Cost = 1;

	// Distance to Player
	UPROPERTY(VisibleAnywhere)
	int32 DistToPlayer;

	// Direction to Player
	UPROPERTY(VisibleAnywhere)
	ECharacterDirection DirToPlayer;

	FGridTileData() : GridCoord(FGridCoord(0, 0)) {}
	FGridTileData(int InX, int InY) : GridCoord(FGridCoord(InX, InY)) {}
};

USTRUCT(BlueprintType)
struct FPathSegment
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FGridCoord First;

	UPROPERTY(VisibleAnywhere)
	FGridCoord Last;

	FPathSegment() : First(INVALID_COORD), Last(INVALID_COORD) {}
	FPathSegment(FGridCoord _First, FGridCoord _Last)
		: First(_First), Last(_Last) {}
};

FORCEINLINE uint32 GetTypeHash(const FPathSegment& Value)
{
	return HashCombine(::GetTypeHash(Value.First), ::GetTypeHash(Value.Last));
}
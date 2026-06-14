// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_GridManager.h"

#include "Camera\CameraComponent.h"
#include "Components\InstancedStaticMeshComponent.h"
#include "Character\GC_Character.h"

// Sets default values
AGC_GridManager::AGC_GridManager()
{
    // Set Components
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
    SetRootComponent(Root);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
    Camera->SetupAttachment(RootComponent);

    // Set InstancedStaticMesh
    Field = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FIELD"));
    Field->SetupAttachment(RootComponent);

    /*OccupiedField = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("OCCUPIEDFIELD"));
    OccupiedField->SetupAttachment(RootComponent);*/
}

void AGC_GridManager::BeginPlay()
{
    Super::BeginPlay();

    GenerateGridTile();

    SetCameraLocation();
}

FGridCoord AGC_GridManager::GetPlayerSpawnCoord() const
{
    // Return Left Center of Grid Field
    return FGridCoord(GridWidth / 2, GridHeight / 2);
}

bool AGC_GridManager::IsInGridBounds(const FGridCoord& Coord) const
{
    // Chck if Coord is between GridWidth and GridHeight
    return Coord.Point.X >= 0 && Coord.Point.X < GridWidth
        && Coord.Point.Y >= 0 && Coord.Point.Y < GridHeight;
}

ECharacterDirection AGC_GridManager::CoordToDirection(const FGridCoord& Coord) const
{
    if (Coord == FGridCoord(0, 1))
    {
        return ECharacterDirection::Up;
    }

    if (Coord == FGridCoord(0, -1))
    {
        return ECharacterDirection::Down;
    }

    if (Coord == FGridCoord(-1, 0))
    {
        return ECharacterDirection::Left;
    }

    if (Coord == FGridCoord(1, 0))
    {
        return ECharacterDirection::Right;
    }

    return ECharacterDirection::None;
}

ECharacterDirection AGC_GridManager::RotatorToDirection(const FRotator& InRotator)
{
    // Determine the Direction Arrording to Rotator's Yaw
    float RotatorYaw = InRotator.Yaw;

    if (RotatorYaw == 180.f)
    {
        return ECharacterDirection::Down;
    }
    else if (RotatorYaw == -90.f)
    {
        return ECharacterDirection::Left;
    }
    else if (RotatorYaw == 90.f)
    {
        return ECharacterDirection::Right;
    }
    else
    {
        return ECharacterDirection::Up;
    }
}

FRotator AGC_GridManager::DirectionToRotator(ECharacterDirection InDirection)
{
    if (InDirection == ECharacterDirection::Down)
    {
        return FRotator(0.f, 180.f, 0.f);
    }
    else if (InDirection == ECharacterDirection::Left)
    {
        return FRotator(0.f, -90.f, 0.f);;
    }
    else if (InDirection == ECharacterDirection::Right)
    {
        return FRotator(0.f, 90.f, 0.f);
    }
    else
    {
        return FRotator(0.f, 0.f, 0.f);
    }
}

FVector AGC_GridManager::CoordToLocation(const FGridCoord& Coord) const
{
    return FVector(GridOrigin.Y + Coord.Point.Y * GridSizeY, GridOrigin.X + Coord.Point.X * GridSizeX, GridFieldZ);
}

FVector AGC_GridManager::CoordToLocation(const FGridCoord& Coord, float Z) const
{
    return FVector(GridOrigin.Y + Coord.Point.Y * GridSizeY, GridOrigin.X + Coord.Point.X * GridSizeX, Z);
}

FGridCoord AGC_GridManager::LocationToCoord(const FVector& Location) const
{
    return FGridCoord(Location.Y / GridSizeY - GridOrigin.Y, Location.X / GridSizeX - GridOrigin.X);
}

int32 AGC_GridManager::CoordToGridTileIndex(const FGridCoord& Coord) const
{
    return Coord.Point.X * GridHeight + Coord.Point.Y;
}

FGridCoord AGC_GridManager::GridTileIndexToCoord(int32 Index) const
{
    int X = Index % GridHeight;
    int Y = Index / GridHeight;

    return FGridCoord(X, Y);
}

bool AGC_GridManager::IsWalkableTile(const FGridCoord& Coord) const
{
    if (!IsInGridBounds(Coord)) return false;

    int32 Index = CoordToGridTileIndex(Coord);
    return GridTileDataArr[Index].bWalkable;
}

bool AGC_GridManager::IsOccupiedTile(const FGridCoord& Coord) const
{
    if (!IsInGridBounds(Coord)) return false;

    int32 Index = CoordToGridTileIndex(Coord);
    return GridTileDataArr[Index].OccupyingActor.IsValid();
}

bool AGC_GridManager::IsCanEnterTile(const FGridCoord& Coord) const
{
    return IsInGridBounds(Coord) && IsWalkableTile(Coord) && !IsOccupiedTile(Coord);
}

AActor* AGC_GridManager::GetOccupyingActor(const FGridCoord& Coord) const
{
    if (!IsInGridBounds(Coord)) return nullptr;

    int32 Index = CoordToGridTileIndex(Coord);
    return GridTileDataArr[Index].OccupyingActor.Get();
}

void AGC_GridManager::SetOccupyingActor(const FGridCoord& Coord, AActor* NewActor)
{
    if (!IsInGridBounds(Coord)) return;

    // Before Set Occupying Actor, Release the Previous Occupation
    if (AGC_Character* Character = Cast<AGC_Character>(NewActor))
    {
        FGridCoord PrevCoord = Character->GetGridCoord();
        
        if (Character == GetOccupyingActor(PrevCoord))
        {
            ClearOccupyingActor(PrevCoord);
        }
    }

    int32 Index = CoordToGridTileIndex(Coord);
    GridTileDataArr[Index].OccupyingActor = NewActor;

    // Set OccupiedField's Instance Opacity To 1
    // OccupiedField->SetCustomDataValue(Index, 0, 1.f, true);
}

void AGC_GridManager::ClearOccupyingActor(const FGridCoord& Coord)
{
    if (!IsInGridBounds(Coord)) return;

    // Release the Occupation
    int32 Index = CoordToGridTileIndex(Coord);
    GridTileDataArr[Index].OccupyingActor = nullptr;

    // Set OccupiedField's Instance Opacity To 0
    // OccupiedField->SetCustomDataValue(Index, 0, 0.f, true);
}

int32 AGC_GridManager::GetDistCoordToCoord(const FGridCoord& CoordA, const FGridCoord& CoordB)
{
    int32 Result = 0;

    Result += FMath::Abs(CoordA.Point.X - CoordB.Point.X);
    Result += FMath::Abs(CoordA.Point.Y - CoordB.Point.Y);

    return Result;
}

int32 AGC_GridManager::GetTileCost(const FGridCoord& Coord) const
{
    if (!IsInGridBounds(Coord)) return MAX_int32;

    int32 Index = CoordToGridTileIndex(Coord);
    return GridTileDataArr[Index].Cost;
}

int32 AGC_GridManager::GetDistToPlayer(const FGridCoord& Coord) const
{
    if (!IsInGridBounds(Coord)) return MAX_int32;

    int32 Index = CoordToGridTileIndex(Coord);
    return GridTileDataArr[Index].DistToPlayer;
}

void AGC_GridManager::SetDistToPlayer(const FGridCoord& Coord, int32 Dist)
{
    if (!IsInGridBounds(Coord)) return;

    int32 Index = CoordToGridTileIndex(Coord);
    GridTileDataArr[Index].DistToPlayer = Dist;
}

ECharacterDirection AGC_GridManager::GetDirToPlayer(const FGridCoord& Coord) const
{
    if (!IsInGridBounds(Coord)) return ECharacterDirection::None;

    int32 Index = CoordToGridTileIndex(Coord);
    return GridTileDataArr[Index].DirToPlayer;
}

void AGC_GridManager::SetDirToPlayer(const FGridCoord& Coord, ECharacterDirection Dir)
{
    if (!IsInGridBounds(Coord)) return;

    int32 Index = CoordToGridTileIndex(Coord);
    GridTileDataArr[Index].DirToPlayer = Dir;
}

ECharacterDirection AGC_GridManager::PathfindToPlayer(const FGridCoord& Start, const FGridCoord& PrevCoord)
{
    double StartTime = FPlatformTime::Seconds();

    if (GetDistToPlayer(Start) <= 1) return ECharacterDirection::None;

    if (IsCanEnterTile(Start + GetDirToPlayer(Start)) &&
        PrevCoord != Start + GetDirToPlayer(Start))
    {
        ECharacterDirection Dir = GetDirToPlayer(Start);

        /*if (CurrentCaseNum < CheckCaseNum)
        {
            double EndTime = FPlatformTime::Seconds();
            double ElapsedMs = (EndTime - StartTime) * 1000.0f;
            UE_LOG(LogTemp, Log, TEXT("FlowField : %.6f ms"), ElapsedMs);
            ++CurrentCaseNum;
            AllSum += ElapsedMs;

            if (CurrentCaseNum >= CheckCaseNum)
            {
                UE_LOG(LogTemp, Warning, TEXT("A* Count : %d"), AstarCount);
                UE_LOG(LogTemp, Warning, TEXT("A* Sum : %.6f"), AstarSum);
                UE_LOG(LogTemp, Warning, TEXT("All Sum : %.6f"), AllSum);
            }
        }*/

        return Dir;
    }
    
    TQueue<FPathSegment> Queue;
    TSet<FGridCoord> Visited;

    int32 BestScore = MAX_int32;
    FGridCoord BestFirst = INVALID_COORD;

    Queue.Enqueue(FPathSegment(INVALID_COORD, Start));

    while (!Queue.IsEmpty())
    {
        FPathSegment Current;
        Queue.Dequeue(Current);

        if (GetDistCoordToCoord(Current.Last, Start) > 2) continue;

        for (FGridCoord Neighbor : GetNeighborCoords(Current.Last))
        {
            if (!IsCanEnterTile(Neighbor) || Visited.Contains(Neighbor)) continue;

            Visited.Add(Neighbor);

            FPathSegment PathSegment;
            PathSegment.Last = Neighbor;

            if (Current.Last == Start)
            {
                PathSegment.First = Neighbor;
            }
            else
            {
                PathSegment.First = Current.First;
            }

            Queue.Enqueue(PathSegment);

            int32 DistToPlayer = GetDistToPlayer(Neighbor);

            if (DistToPlayer <= GetDistToPlayer(Start))
            {
                int32 Score = DistToPlayer;

                if (Neighbor == PrevCoord)
                {
                    Score += 9999;
                }

                if ((Score == BestScore && FMath::RandBool())
                    || Score < BestScore)
                {
                    BestScore = Score;
                    BestFirst = PathSegment.First;
                }
            }
        }
    }

    /*if (CurrentCaseNum < CheckCaseNum)
    {
        double EndTime = FPlatformTime::Seconds();
        double ElapsedMs = (EndTime - StartTime) * 1000.0;
        UE_LOG(LogTemp, Log, TEXT("A* : %.6f ms"), ElapsedMs);

        AstarSum += ElapsedMs;
        AllSum += ElapsedMs;
        ++AstarCount;
        ++CurrentCaseNum;

        if (CurrentCaseNum >= CheckCaseNum)
        {
            UE_LOG(LogTemp, Warning, TEXT("A* Count : %d"), AstarCount);
            UE_LOG(LogTemp, Warning, TEXT("A* Sum : %.6f"), AstarSum);
            UE_LOG(LogTemp, Warning, TEXT("All Sum : %.6f"), AllSum);
        }
    }*/

    return CoordToDirection(BestFirst - Start);
}

TArray<FGridCoord> AGC_GridManager::GetAllCoords()
{
    TArray<FGridCoord> Result;

    for (int32 x = 0; x < GridWidth; ++x)
    {
        for (int32 y = 0; y < GridHeight; ++y)
        {
            Result.Add(FGridCoord(x, y));
        }
    }

    return Result;
}

TArray<FGridCoord> AGC_GridManager::FilterEmptyCoords(const TArray<FGridCoord>& Coords)
{
    TArray<FGridCoord> Result;

    // Filter only Unoccupied Tiles
    for (const FGridCoord& Coord : Coords)
    {
        if (!IsInGridBounds(Coord)) continue;
        if (IsOccupiedTile(Coord)) continue;

        Result.Add(Coord);
    }

    return Result;
}

FGridCoord AGC_GridManager::RotateCoordOffset(const FGridCoord& Coord, ECharacterDirection Dir)
{
    switch (Dir)
    {
    case ECharacterDirection::Down:
        return FGridCoord(Coord.Point.X, -Coord.Point.Y);
    case ECharacterDirection::Left:
        return FGridCoord(-Coord.Point.Y, Coord.Point.X);
    case ECharacterDirection::Right:
        return FGridCoord(Coord.Point.Y, -Coord.Point.X);
    default:
        return Coord;
    }
}

TArray<FGridCoord> AGC_GridManager::GetNeighborCoords(const FGridCoord& Coord) const
{
    TArray<FGridCoord> Neighbors;

    if (IsInGridBounds(Coord + ECharacterDirection::Up))
    {
        Neighbors.Add(Coord + ECharacterDirection::Up);
    }
    if (IsInGridBounds(Coord + ECharacterDirection::Down))
    {
        Neighbors.Add(Coord + ECharacterDirection::Down);
    }
    if (IsInGridBounds(Coord + ECharacterDirection::Left))
    {
        Neighbors.Add(Coord + ECharacterDirection::Left);
    }
    if (IsInGridBounds(Coord + ECharacterDirection::Right))
    {
        Neighbors.Add(Coord + ECharacterDirection::Right);
    }

    return Neighbors;
}

void AGC_GridManager::RebuildPathfindField(const FGridCoord& Target)
{
    BuildDistanceField(Target);
    BuildDirectionField();
}

void AGC_GridManager::GenerateGridTile()
{
    if (!Field || !Field->GetStaticMesh())
    {
        UE_LOG(LogTemp, Warning, TEXT("No ISM StaticMesh"));
    }

    // Clear And Init ISMs
    Field->ClearInstances();
    /*OccupiedField->ClearInstances();
    OccupiedField->NumCustomDataFloats = 1;*/

    // Generate Tiles for Width, Height
    for (int32 x = 0; x < GridWidth; ++x)
    {
        for (int32 y = 0; y < GridHeight; ++y)
        {
            // Set Tile Location And Scale According to Coord
            FTransform Transform;
            Transform.SetLocation(CoordToLocation(FGridCoord(x, y), GridOrigin.Z));
            Transform.SetScale3D(FVector(GridSizeY / WorldGridSizeY, GridSizeX / WorldGridSizeX, 1.f));

            // Add Instance of ISM Based on Index of Tiles
            int32 Index = Field->AddInstance(Transform);
            // OccupiedField->AddInstance(Transform);

            // Set OccupiedField's Instance Opacity To 0
            // OccupiedField->SetCustomDataValue(Index, 0, 0.f, true);

            // Add GridTileData
            GridTileDataArr.Add(FGridTileData(x, y));
        }
    }
}

void AGC_GridManager::SetCameraLocation()
{
    // Set CameraLocation According to GridWidth and GridSizeX
    FVector NewLocation = Camera->GetRelativeLocation();
    NewLocation.Y = (GridWidth - 1) * (GridSizeX / 2);

    Camera->SetRelativeLocation(NewLocation);
}

void AGC_GridManager::BuildDistanceField(const FGridCoord& Target)
{
    ResetDistanceField();

    TQueue<FGridCoord> Queue;
    SetDistToPlayer(Target, 0);
    Queue.Enqueue(Target);

    while (!Queue.IsEmpty())
    {
        FGridCoord Current;
        Queue.Dequeue(Current);

        for (FGridCoord Next : GetNeighborCoords(Current))
        {
            if (!IsWalkableTile(Next)) continue;
            
            int32 NewDist = GetDistToPlayer(Current) + GetTileCost(Next);

            if (NewDist < GetDistToPlayer(Next))
            {
                SetDistToPlayer(Next, NewDist);
                Queue.Enqueue(Next);
            }
        }
    }
}

void AGC_GridManager::ResetDistanceField()
{
    for (FGridTileData Tile : GridTileDataArr)
    {
        FGridCoord Coord = Tile.GridCoord;
        SetDistToPlayer(Coord, MAX_int32);
    }
}

void AGC_GridManager::BuildDirectionField()
{
    for (FGridTileData Tile : GridTileDataArr)
    {
        FGridCoord Coord = Tile.GridCoord;
        int32 BestDist = MAX_int32;
        ECharacterDirection BestDir = ECharacterDirection::None;

        for (FGridCoord Neighbor : GetNeighborCoords(Coord))
        {
            if (!IsWalkableTile(Neighbor)) continue;

            int32 Dist = GetDistToPlayer(Neighbor);

            if (Dist < BestDist)
            {
                BestDist = Dist;
                BestDir = CoordToDirection(Neighbor - Coord);
            }
        }

        SetDirToPlayer(Coord, BestDir);
    }
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Game\GC_Grid.h"
#include "Skill\GC_Skill.h"

#include "GC_Projectile.generated.h"

class AGC_GridManager;
class AGC_Character;
class UGC_SkillEffect;

USTRUCT(BlueprintType)
struct FProjectileParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	int32 Range;

	UPROPERTY(EditAnywhere)
	bool bPiercing;

	UPROPERTY()
	TArray<TObjectPtr<UGC_SkillEffect>> HitEffects;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UGC_SkillEffect>> EndEffects;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> ProjectileMesh;
};

UCLASS()
class GRIDCRAWLER_API AGC_Projectile : public AActor
{
	GENERATED_BODY()

public:
	AGC_Projectile();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	void Init(const FSkillContext& Context, const FProjectileParam& InParam);

	UStaticMeshComponent* GetStaticMesh() const
	{
		return StaticMesh;
	}

protected:
	void MoveStep();

	void ProcessHitEffect(AGC_Character* HitActor);

	void ProcessEndEffect();

protected:
	/** Components & Manager */
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AGC_GridManager> GridManager = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AGC_Character> User = nullptr;

	/** Move */
	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	ECharacterDirection ProjectileDirection;

	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	FGridCoord CurrentCoord;

	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	FGridCoord EndCoord;

	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	FVector EndLocation;

	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	int32 CurrentRange;

	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	TArray<FGridCoord> ProjectileMoveArea;

	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	bool bIsMove = false;

	/** Projectile Param & Context */
	UPROPERTY(VisibleAnywhere, Category = "GC|Param")
	FProjectileParam ProjectileParam;

	UPROPERTY(VisibleAnywhere, Category = "GC|Context")
	FSkillContext SkillContext;

	UPROPERTY(VisibleAnywhere, Category = "GC|Context");
	TArray<TObjectPtr<AGC_Character>> HitActors;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Character\GC_CharacterStat.h"

#include "GC_EnemyData.generated.h"

class AGC_CharacterEnemy;
class UGC_SkillData;

UCLASS()
class GRIDCRAWLER_API UGC_EnemyData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGC_CharacterEnemy> EnemyClass;
	
	UPROPERTY(EditAnywhere)
	FCharacterBaseStats Stats;

	UPROPERTY(EditAnywhere)
	int32 Exp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> AnimClass;

	// Behavior
	UPROPERTY(EditAnywhere)
	float DecisionInterval = 0.2f;

	UPROPERTY(EditAnywhere)
	TArray<const UGC_SkillData*> Skills;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere)
	UStaticMesh* LeftHandMesh;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RightHandMesh;

	UPROPERTY(EditAnywhere)

	FRotator LeftHandMeshRotator;

	UPROPERTY(EditAnywhere)
	FRotator RightHandMeshRotator;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage;
};

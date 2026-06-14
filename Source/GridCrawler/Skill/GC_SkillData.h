// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Skill\GC_SkillDelivery.h"

#include "GC_SkillData.generated.h"

UCLASS(BlueprintType)
class GRIDCRAWLER_API UGC_SkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> SkillIcon;

	UPROPERTY(EditAnywhere)
	FName SkillName;

	UPROPERTY(EditAnywhere)
	FName SkillDescription;

	UPROPERTY(EditAnywhere)
	float SkillCooldown;

	UPROPERTY(EditAnywhere)
	TArray<UGC_SkillDelivery*> Deliveries;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> SkillWeaponHandL;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> SkillWeaponHandR;

	UPROPERTY(EditAnywhere)
	FRotator SocketRotatorL;

	UPROPERTY(EditAnywhere)
	FRotator SocketRotatorR;
};

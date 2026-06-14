// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GC_SkillDelivery.h"

#include "Skill\GC_Projectile.h"

#include "GC_DeliveryProjectile.generated.h"

UCLASS()
class GRIDCRAWLER_API UGC_DeliveryProjectile : public UGC_SkillDelivery
{
	GENERATED_BODY()
	
public:
	virtual void Execute(const FSkillContext& Context) override;

	virtual void GetSkillIndicator(const FSkillContext& Context, TArray<FGridCoord>& Result) override;

protected:

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGC_Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	FProjectileParam ProjectileParam;
};

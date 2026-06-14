// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GC_AnimInstance.generated.h"

class AGC_Character;

UCLASS()
class GRIDCRAWLER_API UGC_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UGC_AnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void SetActorDeath();

protected:
	UPROPERTY(VisibleAnywhere, Category = "GC|Character")
	TObjectPtr<AGC_Character> Character = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "GC|Movement");
	uint8 bIsDeath : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GC|Movement")
	uint8 bIsMove : 1;

	UPROPERTY(VisibleAnywhere, Category = "GC|Movement")
	FVector LastLocation;

	UPROPERTY(VisibleAnywhere, Category = "GC|Movement")
	float Speed;

	UPROPERTY(VisibleAnywhere, Category = "GC|Movement")
	float MoveThreshold;
};

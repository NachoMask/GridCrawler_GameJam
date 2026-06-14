// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/GC_AnimInstance.h"

#include "Character\GC_Character.h"

UGC_AnimInstance::UGC_AnimInstance()
{
	MoveThreshold = 0.f;
}

void UGC_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AGC_Character>(GetOwningActor());

	if (Character)
	{
		bIsDeath = false;
		LastLocation = Character->GetActorLocation();
	}
}

void UGC_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
		FVector CurrentLocation = Character->GetActorLocation();
		Speed = (CurrentLocation - LastLocation).Size() / DeltaSeconds;

		bIsMove = Speed > MoveThreshold;

		LastLocation = CurrentLocation;
	}
}

void UGC_AnimInstance::SetActorDeath()
{
	bIsDeath = true;
}

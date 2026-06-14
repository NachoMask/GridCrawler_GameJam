// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GC_AnimNotify_SkillEffect_0.generated.h"

UCLASS()
class GRIDCRAWLER_API UGC_AnimNotify_SkillEffect_0 : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	int32 StepIndex = 0;
};

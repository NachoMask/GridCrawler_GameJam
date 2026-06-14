// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GC_AnimNotify_SkillEffect_1.h"

#include "Character\GC_Character.h"

void UGC_AnimNotify_SkillEffect_1::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AGC_Character* Character = Cast<AGC_Character>(MeshComp->GetOwner());
		if (Character)
		{
			Character->RequestSkillStep(StepIndex);
		}
	}
}

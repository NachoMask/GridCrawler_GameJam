// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GC_FadeWidget.h"

#include "Animation\WidgetAnimation.h"
#include "Animation\UMGSequencePlayer.h"

void UGC_FadeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FWidgetAnimationDynamicEvent FadeOutEvent;
	FadeOutEvent.BindDynamic(this, &UGC_FadeWidget::BroadcastFadeOut);
	BindToAnimationFinished(FadeToBlack, FadeOutEvent);

	FWidgetAnimationDynamicEvent FadeInEvent;
	FadeOutEvent.BindDynamic(this, &UGC_FadeWidget::BroadcastFadeIn);
	BindToAnimationFinished(FadeFromBlack, FadeInEvent);
}

void UGC_FadeWidget::PlayFadeOut(float Time)
{
	if (FadeToBlack)
	{
		float OriginalLength = FadeToBlack->GetEndTime();
		float Speed = OriginalLength / Time;

		FWidgetAnimationHandle AnimationHandle = PlayAnimation(FadeToBlack);
		AnimationHandle.GetAnimationState()->SetPlaybackSpeed(Speed);
	}
}

void UGC_FadeWidget::PlayFadeIn(float Time)
{
	if (FadeToBlack)
	{
		float OriginalLength = FadeFromBlack->GetEndTime();
		float Speed = OriginalLength / Time;

		FWidgetAnimationHandle AnimationHandle = PlayAnimation(FadeFromBlack);
		AnimationHandle.GetAnimationState()->SetPlaybackSpeed(Speed);
	}
}

void UGC_FadeWidget::BroadcastFadeOut()
{
	OnFadeOut.Broadcast();
}

void UGC_FadeWidget::BroadcastFadeIn()
{
	OnFadeIn.Broadcast();
}

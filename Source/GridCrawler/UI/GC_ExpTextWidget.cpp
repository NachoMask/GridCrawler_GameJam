// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_ExpTextWidget.h"

#include "Kismet\GameplayStatics.h"
#include "Components\TextBlock.h"

void UGC_ExpTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ExpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ExpText")));
	ensure(ExpText);

	FWidgetAnimationDynamicEvent Event;
	Event.BindDynamic(this, &UGC_ExpTextWidget::ExpTextFadeOut);
	BindToAnimationFinished(FadeOut, Event);

	PlayAnimation(FadeOut);
	bAnimationPaused = false;
}

void UGC_ExpTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetWorld())
	{
		if (UGameplayStatics::IsGamePaused(GetWorld()))
		{
			PausedTime = PauseAnimation(FadeOut);
			bAnimationPaused = true;
		}
		else if (bAnimationPaused)
		{
			PlayAnimation(FadeOut, PausedTime);
			bAnimationPaused = false;
		}
	}
}

void UGC_ExpTextWidget::SetExpText(int32 Exp)
{
	if (ExpText)
	{
		ExpText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Exp)));
	}
}

void UGC_ExpTextWidget::ExpTextFadeOut()
{
	OnExpTextFadeOut.Broadcast();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_DamageTextWidget.h"

#include "Kismet\GameplayStatics.h"
#include "Components\TextBlock.h"

void UGC_DamageTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DamageText")));
	ensure(DamageText);

	FWidgetAnimationDynamicEvent Event;
	Event.BindDynamic(this, &UGC_DamageTextWidget::DamageTextFadeOut);
	BindToAnimationFinished(FadeOut, Event);

	PlayAnimation(FadeOut);
	bAnimationPaused = false;
}

void UGC_DamageTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetWorld())
	{
		if (UGameplayStatics::IsGamePaused(GetWorld()))
		{
			PauseTime = PauseAnimation(FadeOut);
			bAnimationPaused = true;
		}
		else if (bAnimationPaused)
		{
			PlayAnimation(FadeOut, PauseTime);
			bAnimationPaused = false;
		}
	}
}

void UGC_DamageTextWidget::SetDamageText(int32 InDamageText)
{
	FText Text = FText::FromString(FString::Printf(TEXT("%d"), InDamageText));
	DamageText->SetText(Text);
}

void UGC_DamageTextWidget::DamageTextFadeOut()
{
	OnDamageTextFadeOut.Broadcast();
}

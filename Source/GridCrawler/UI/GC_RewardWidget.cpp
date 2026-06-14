// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_RewardWidget.h"

#include "Components\Border.h"
#include "Components\TextBlock.h"
#include "Components\Image.h"

void UGC_RewardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CursorBorder = Cast<UBorder>(GetWidgetFromName(TEXT("CursorBorder")));
	ensure(CursorBorder);

	RewardTypeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RewardTypeText")));
	ensure(RewardTypeText);
	RewardTypeText->SetVisibility(ESlateVisibility::Hidden);

	RewardIcon = Cast<UImage>(GetWidgetFromName(TEXT("RewardIcon")));
	ensure(RewardIcon);

	RewardNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RewardNameText")));
	ensure(RewardNameText);

	RewardSubText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RewardSubText")));
	ensure(RewardSubText);

	RewardDescriptionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RewardDescriptionText")));
	ensure(RewardDescriptionText);

	FWidgetAnimationDynamicEvent Event;
	Event.BindDynamic(this, &UGC_RewardWidget::RewardShowEnd);
	BindToAnimationFinished(ShowReward, Event);

	PlayAnimation(ShowReward);
}

void UGC_RewardWidget::SetRewardTypeText(FText Text)
{
	if (RewardTypeText)
	{
		RewardTypeText->SetText(Text);
	}
}

void UGC_RewardWidget::SetRewardIcon(UTexture2D* Icon)
{
	if (RewardIcon)
	{
		if (Icon)
		{
			RewardIcon->SetVisibility(ESlateVisibility::Visible);
			RewardIcon->SetBrushFromTexture(Icon);
		}
		else
		{
			RewardIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UGC_RewardWidget::SetRewardNameText(FText Text)
{
	if (RewardNameText)
	{
		RewardNameText->SetText(Text);
	}
}

void UGC_RewardWidget::SetRewardSubText(FText Text)
{
	if (RewardSubText)
	{
		RewardSubText->SetText(Text);
	}
}

void UGC_RewardWidget::SetRewardDescriptionText(FText Text)
{
	if (RewardDescriptionText)
	{
		RewardDescriptionText->SetText(Text);
	}
}

void UGC_RewardWidget::SetRewardOutCursor()
{
	if (CursorBorder)
	{
		CursorBorder->SetRenderScale(FVector2D(1.f, 1.f));
		CursorBorder->SetBrushColor((FLinearColor(0.02f, 0.02f, 0.02f, 1.f)));
	}
}

void UGC_RewardWidget::SetRewardInCursor()
{
	if (CursorBorder)
	{
		CursorBorder->SetRenderScale(FVector2D(1.1f, 1.1f));
		CursorBorder->SetBrushColor((FLinearColor(1.f, 1.f, 1.f, 1.f)));
	}
}

void UGC_RewardWidget::RewardShowEnd()
{
	OnRewardShowEnd.Broadcast();
}



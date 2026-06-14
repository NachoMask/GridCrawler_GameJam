// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_TimerWidget.h"

#include "Components\TextBlock.h"

void UGC_TimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MinutesText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MinutesText")));
	ensure(MinutesText);

	SecondsText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SecondsText")));
	ensure(SecondsText);
}

void UGC_TimerWidget::SetTimerText(int32 TotalSeconds)
{
	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;

	FText MinutesTextValue;
	FText SecondsTextValue;
	if (Minutes >= 10)
	{
		MinutesTextValue = FText::FromString(FString::Printf(TEXT("%d"), Minutes));
	}
	else
	{
		MinutesTextValue = FText::FromString(FString::Printf(TEXT("0%d"), Minutes));
	}

	if (Seconds >= 10)
	{
		SecondsTextValue = FText::FromString(FString::Printf(TEXT("%d"), Seconds));
	}
	else
	{
		SecondsTextValue = FText::FromString(FString::Printf(TEXT("0%d"), Seconds));
	}

	MinutesText->SetText(MinutesTextValue);
	SecondsText->SetText(SecondsTextValue);
}

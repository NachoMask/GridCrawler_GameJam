// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_ExpHUDWidget.h"

#include "Components\ProgressBar.h"
#include "Components\TextBlock.h"

void UGC_ExpHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
	ensure(LevelText);

	ExpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ExpProgressBar")));
	ensure(ExpProgressBar);
}

void UGC_ExpHUDWidget::SetLevelText(int32 Level)
{
	if (LevelText)
	{
		FText Text = FText::FromString(FString::Printf(TEXT("%d"), Level));
		LevelText->SetText(Text);
	}
}

void UGC_ExpHUDWidget::SetExpProgressBar(int32 CurrentExp, int32 MaxExp)
{
	if (ExpProgressBar)
	{
		ExpProgressBar->SetPercent(CurrentExp / float(MaxExp));
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_LvExpWidget.h"

#include "Components\ProgressBar.h"
#include "Components\TextBlock.h"

void UGC_LvExpWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGC_LvExpWidget::SetExpBar(int32 CurrentExp, int32 MaxExp)
{
	if (ExpBar)
	{
		ExpBar->SetPercent(CurrentExp / (float)MaxExp);
	}
}

void UGC_LvExpWidget::SetLvText(int32 Lv)
{
	if (LvText)
	{
		LvText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Lv)));
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_TitleGameMode.h"

#include "UI\GC_TitleWidget.h"

void AGC_TitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (TitleWidgetClass)
	{
		TitleWidget = CreateWidget<UGC_TitleWidget>(GetWorld(), TitleWidgetClass);
		TitleWidget->AddToViewport();
	}
}

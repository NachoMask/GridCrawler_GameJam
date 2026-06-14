// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GC_TitlePlayerController.h"

#include "Kismet\GameplayStatics.h"
#include "UI\GC_TitleWidget.h"
#include "UI\GC_FadeWidget.h"

void AGC_TitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(FadeWidgetClass);
	FadeWidget = CreateWidget<UGC_FadeWidget>(this, FadeWidgetClass);
	if (FadeWidget)
	{
		FadeWidget->AddToViewport(9999);
		FadeIn(1.f);
	}

	check(TitleWidgetClass);
	TitleWidget = CreateWidget<UGC_TitleWidget>(this, TitleWidgetClass);
	if (TitleWidget)
	{
		TitleWidget->AddToViewport(0);
		TitleWidget->SetTitleStep(CurrentStep);
	}
}

void AGC_TitlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindKey(
		EKeys::SpaceBar,
		EInputEvent::IE_Pressed,
		this, &AGC_TitlePlayerController::OnStartKeyPressed);
}

void AGC_TitlePlayerController::OnStartKeyPressed()
{
	if (bScreenTransitioning) return;

	check(TitleWidget);
	check(FadeWidget);

	switch (CurrentStep)
	{
	case 0:	// 0: Logo Anim
	{
		// Logo Anim Forced End
		CurrentStep = 1;
		TitleWidget->SetTitleStep(CurrentStep);
		break;
	}
	case 1: // 1: Title
	{
		bScreenTransitioning = true;
		CurrentStep = 2;
		TitleWidget->SetTitleStep(CurrentStep);
		FadeOut(1.f);
		break;
	}
	}
}

void AGC_TitlePlayerController::FadeOut(float FadeTime)
{
	bScreenTransitioning = true;

	if (FadeWidget)
	{
		FadeWidget->PlayFadeOut(FadeTime);
	}

	FTimerHandle RestartHandle;
	GetWorld()->GetTimerManager().SetTimer(
		RestartHandle, this, &AGC_TitlePlayerController::OnFadeOutFinished, FadeTime, false);
}

void AGC_TitlePlayerController::FadeIn(float FadeTime)
{
	bScreenTransitioning = true;

	if (FadeWidget)
	{
		FadeWidget->PlayFadeIn(FadeTime);
	}

	FTimerHandle RestartHandle;
	GetWorld()->GetTimerManager().SetTimer(
		RestartHandle, this, &AGC_TitlePlayerController::OnFadeInFinished, FadeTime, false);
}

void AGC_TitlePlayerController::OnFadeOutFinished()
{
	UGameplayStatics::OpenLevel(this, TEXT("BattleMap"));
}

void AGC_TitlePlayerController::OnFadeInFinished()
{
	bScreenTransitioning = false;

	if (CurrentStep == 0)
	{
		CurrentStep = 1;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_TitleWidget.h"

#include "Animation\WidgetAnimation.h"
#include "Components\TextBlock.h"
#include "Components\CanvasPanelSlot.h"

void UGC_TitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LogoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LogoText")));
	ensure(LogoText);

	PressSpaceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PressSpaceText")));
	ensure(PressSpaceText);

	ensure(LogoAnim);
	FWidgetAnimationDynamicEvent LogoFadeInEvent;
	LogoFadeInEvent.BindDynamic(this, &UGC_TitleWidget::OnLogoFadeInFinished);

	BindToAnimationFinished(LogoAnim, LogoFadeInEvent);
}

void UGC_TitleWidget::SetTitleStep(int32 Step)
{
	UE_LOG(LogTemp, Warning, TEXT("Step : %d"), Step);

	switch (Step)
	{
	case 0:	// Logo Animation
	{
		PlayAnimation(LogoAnim);
		break;
	}
	case 1:	// Title
	{
		OnLogoFadeInFinished();
		break;
	}
	case 2:	// Title End
	{
		PlayAnimation(PressSpaceAnim2);
		break;
	}
	}
}

void UGC_TitleWidget::OnLogoFadeInFinished()
{
	ensure(PressSpaceAnim);

	FLinearColor Color = FLinearColor::White;
	Color.A = 1.f;
	LogoText->SetColorAndOpacity(Color);

	PlayAnimation(PressSpaceAnim, 0.f, 0);
}

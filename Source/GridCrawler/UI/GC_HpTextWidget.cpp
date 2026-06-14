// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_HpTextWidget.h"

#include "Components\TextBlock.h"

UGC_HpTextWidget::UGC_HpTextWidget()
{

}

void UGC_HpTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpText")));
	ensure(HpText);
}

void UGC_HpTextWidget::UpdateHpText(float CurrentHp)
{
	if (HpText)
	{
		HpText->SetText(FText::FromString(
			FString::Printf(TEXT("%.0f"), CurrentHp)
		));
	}
}


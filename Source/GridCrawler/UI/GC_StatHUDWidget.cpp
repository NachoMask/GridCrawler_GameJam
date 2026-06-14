// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_StatHUDWidget.h"

#include "Components\TextBlock.h"
#include "Components\ProgressBar.h"

void UGC_StatHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGC_StatHUDWidget::SetCurrentHpText(float CurrentHp)
{
	if (CurrentHpText)
	{
		FText Text = FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHp));
		CurrentHpText->SetText(Text);
	}
}

void UGC_StatHUDWidget::SetMaxHpText(float MaxHp)
{
	if (MaxHpText)
	{
		FText Text = FText::FromString(FString::Printf(TEXT("%.0f"), MaxHp));
		MaxHpText->SetText(Text);
	}
}

void UGC_StatHUDWidget::SetAttackText(float Attack)
{
	if (AttackText)
	{
		FText Text = FText::FromString(FString::Printf(TEXT("%.0f"), Attack));
		AttackText->SetText(Text);
	}
}

void UGC_StatHUDWidget::SetArmorText(float Armor)
{
	if (ArmorText)
	{
		FText Text = FText::FromString(FString::Printf(TEXT("%.0f"), Armor));
		ArmorText->SetText(Text);
	}
}

void UGC_StatHUDWidget::SetMoveSpeedText(float MoveSpeed)
{
	if (MoveSpeedText)
	{
		FText Text = FText::FromString(FString::Printf(TEXT("%.0f"), MoveSpeed));
		MoveSpeedText->SetText(Text);
	}
}
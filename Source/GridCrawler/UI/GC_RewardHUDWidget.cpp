// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_RewardHUDWidget.h"

#include "Components\HorizontalBox.h"
#include "UI\GC_RewardWidget.h"

void UGC_RewardHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RewardHUDBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("RewardHUDBox")));
	ensure(RewardHUDBox);
}

void UGC_RewardHUDWidget::SetRewardHUDVisible(bool bVisible)
{
	if (RewardHUDBox)
	{
		if (bVisible)
		{
			RewardHUDBox->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			RewardHUDBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UGC_RewardHUDWidget::SetRewardRemove()
{
	for (UGC_RewardWidget* RewardWidget : RewardWidgets)
	{
		RewardHUDBox->RemoveChild(RewardWidget);
	}
	RewardWidgets.Empty();
	ShowEndRewardNum = 0;

	SetRewardHUDVisible(false);
}

void UGC_RewardHUDWidget::SetRewardHUD_Skill(UTexture2D* InSkillIcon, FName InSkillName, float InSkillCooldown, FName InSkillDesc)
{
	check(RewardWidgetClass);

	UGC_RewardWidget* RewardWidget = CreateWidget<UGC_RewardWidget>(this, RewardWidgetClass);

	FText RewardTypeText = FText::FromName(TEXT("New Skill"));
	UTexture2D* SkillIcon = InSkillIcon;
	FText SkillName = FText::FromName(InSkillName);
	FText SkillCooldown = FText::FromString(FString::Printf(TEXT("%.0f sec"), InSkillCooldown));
	FText SkillDesc = FText::FromName(InSkillDesc);

	RewardWidget->SetRewardTypeText(RewardTypeText);
	RewardWidget->SetRewardIcon(SkillIcon);
	RewardWidget->SetRewardNameText(SkillName);
	RewardWidget->SetRewardSubText(SkillCooldown);
	RewardWidget->SetRewardDescriptionText(SkillDesc);

	RewardWidget->OnRewardShowEnd.AddUObject(this, &UGC_RewardHUDWidget::CheckAllRewardShowEnd);

	RewardHUDBox->AddChild(RewardWidget);
	RewardWidgets.Add(RewardWidget);

	SetRewardHUDVisible(true);
}

void UGC_RewardHUDWidget::SetRewardHUD_Stat(int32 StatEnumIndex, float InStatValue)
{
	check(RewardWidgetClass);

	UGC_RewardWidget* RewardWidget = CreateWidget<UGC_RewardWidget>(this, RewardWidgetClass);

	FText RewardTypeText = FText::FromName(TEXT("Stat Upgrade"));
	UTexture2D* StatIcon = nullptr;
	FText StatName;
	if (StatNameArr.IsValidIndex(StatEnumIndex))
	{
		StatName = FText::FromString(FString::Printf(
			TEXT("%s \n+ %.0f"), *StatNameArr[StatEnumIndex].ToString(), InStatValue));
	}
	FText BlankSubText;
	FText BlankDesc;

	RewardWidget->SetRewardTypeText(RewardTypeText);
	RewardWidget->SetRewardIcon(StatIcon);
	RewardWidget->SetRewardNameText(StatName);
	RewardWidget->SetRewardSubText(BlankSubText);
	RewardWidget->SetRewardDescriptionText(BlankDesc);

	RewardWidget->OnRewardShowEnd.AddUObject(this, &UGC_RewardHUDWidget::CheckAllRewardShowEnd);

	RewardHUDBox->AddChild(RewardWidget);
	RewardWidgets.Add(RewardWidget);

	SetRewardHUDVisible(true);
}

void UGC_RewardHUDWidget::SetRewardHUDOutCursor(int32 Index)
{
	if (RewardWidgets.IsValidIndex(Index))
	{
		RewardWidgets[Index]->SetRewardOutCursor();
	}
}

void UGC_RewardHUDWidget::SetRewardHUDInCursor(int32 Index)
{
	if (RewardWidgets.IsValidIndex(Index))
	{
		RewardWidgets[Index]->SetRewardInCursor();
	}
}

void UGC_RewardHUDWidget::CheckAllRewardShowEnd()
{
	++ShowEndRewardNum;

	if (ShowEndRewardNum == RewardWidgets.Num())
	{
		OnAllRewardShowEnd.Broadcast();
	}
}


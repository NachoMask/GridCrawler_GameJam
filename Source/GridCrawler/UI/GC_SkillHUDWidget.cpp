// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_SkillHUDWidget.h"

#include "Components\HorizontalBox.h"
#include "UI\GC_SkillWidget.h"

void UGC_SkillHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillHUDBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("SkillHUDBox")));
	
	SkillWidgets.Empty();
	if (SkillHUDBox && SkillWidgetClass)
	{
		for (int32 i = 0; i < SkillNum; ++i)
		{
			UGC_SkillWidget* SkillWidget = CreateWidget<UGC_SkillWidget>(this, SkillWidgetClass);
			if (SkillWidget)
			{
				FText SkillKey = (SkillKeyArr.IsValidIndex(i) ? SkillKeyArr[i] : FText::FromString(TEXT("")));
				SkillWidget->SetSkillKey(SkillKey);
				SkillHUDBox->AddChild(SkillWidget);
				SkillWidgets.Add(SkillWidget);
			}
		}

		if (DashWidgetClass)
		{
			UGC_SkillWidget* DashWidget = CreateWidget<UGC_SkillWidget>(this, DashWidgetClass);
			if (DashWidget)
			{
				SkillHUDBox->AddChild(DashWidget);
				SkillWidgets.Add(DashWidget);
			}
		}
	}
}

void UGC_SkillHUDWidget::UpdateSkillWidget(int32 SkillIndex, UTexture2D* InSkillIcon)
{
	if (SkillWidgets.IsValidIndex(SkillIndex))
	{
		SkillWidgets[SkillIndex]->SetSkillData(InSkillIcon);
	}
}

void UGC_SkillHUDWidget::SetSkillInUse(int32 SkillIndex)
{
	if (SkillWidgets.IsValidIndex(SkillIndex))
	{
		SkillWidgets[SkillIndex]->SetSkillInUse();
	}
}

void UGC_SkillHUDWidget::SetSkillCooldown(int32 SkillIndex, float CooldownTime)
{
	if (SkillWidgets.IsValidIndex(SkillIndex))
	{
		SkillWidgets[SkillIndex]->SetSkillCooldown(CooldownTime);
	}
}



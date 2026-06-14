// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_SkillWidget.h"

#include "Kismet\GameplayStatics.h"
#include "Components\TextBlock.h"
#include "Components\Image.h"
#include "Components\ProgressBar.h"

void UGC_SkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(SkillCooldownBar);
	SkillCooldownBar->SetVisibility(ESlateVisibility::Hidden);

	ensure(SkillCooldownText);
	SkillCooldownText->SetVisibility(ESlateVisibility::Hidden);
}

void UGC_SkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetWorld() && UGameplayStatics::IsGamePaused(GetWorld())) return;

	if (SkillCooldownBar && SkillCooldownText && Cooldown > 0.f)
	{
		Cooldown -= InDeltaTime;

		SkillCooldownBar->SetPercent(Cooldown / MaxCooldown);

		FText CooldownText;
		if (Cooldown >= 1.f)
		{
			CooldownText = FText::FromString(FString::Printf(TEXT("%.0f"), Cooldown));
		}
		else
		{
			CooldownText = FText::FromString(FString::Printf(TEXT("%.1f"), Cooldown));
		}
		
		SkillCooldownText->SetText(CooldownText);

		if (Cooldown <= 0.f)
		{
			SkillCooldownBar->SetVisibility(ESlateVisibility::Hidden);
			SkillCooldownBar->SetPercent(1.f);
			SkillCooldownText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}

void UGC_SkillWidget::SetSkillKey(FText KeyText)
{
	if (SkillKey)
	{
		SkillKey->SetText(KeyText);
	}
}

void UGC_SkillWidget::SetSkillData(UTexture2D* InSkillIcon)
{
	// UE_LOG(LogTemp, Log, TEXT("SkillWidget : SetSkillData"));

	if (SkillIcon && InSkillIcon)
	{
		SkillIcon->SetBrushFromTexture(InSkillIcon);
	}
}

void UGC_SkillWidget::SetSkillInUse()
{
	if (SkillCooldownBar)
	{
		SkillCooldownBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGC_SkillWidget::SetSkillCooldown(float InCooldown)
{
	MaxCooldown = InCooldown;
	Cooldown = MaxCooldown;

	if (SkillCooldownBar)
	{
		if (Cooldown > 0.f)
		{
			SkillCooldownBar->SetVisibility(ESlateVisibility::Visible);
			SkillCooldownBar->SetPercent(Cooldown / MaxCooldown);

			SkillCooldownText->SetVisibility(ESlateVisibility::Visible);
			FText CooldownText = FText::FromString(FString::Printf(TEXT("%.0f"), Cooldown));
			SkillCooldownText->SetText(CooldownText);
		}
		else
		{
			SkillCooldownBar->SetVisibility(ESlateVisibility::Hidden);
			SkillCooldownText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

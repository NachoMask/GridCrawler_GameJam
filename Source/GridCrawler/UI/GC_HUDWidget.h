// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_HUDWidget.generated.h"

class UGC_TimerWidget;
class UGC_SkillHUDWidget;
class UGC_LvExpWidget;

UCLASS()
class GRIDCRAWLER_API UGC_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	/** Timer */
	UGC_TimerWidget* GetTimerWidget() const
	{
		return TimerWidget;
	}
	
	/** SkillHUD */
	UGC_SkillHUDWidget* GetSkillHUDWidget() const
	{
		return SkillHUDWidget;
	}

	/** LvExpHUD */
	UGC_LvExpWidget* GetLvExpWidget() const
	{
		return LvExpWidget;
	}

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGC_TimerWidget> TimerWidget = nullptr;

	/*UPROPERTY()
	TObjectPtr<UGC_StatHUDWidget> StatHUDWidget = nullptr;*/

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGC_SkillHUDWidget> SkillHUDWidget = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGC_LvExpWidget> LvExpWidget = nullptr;
};

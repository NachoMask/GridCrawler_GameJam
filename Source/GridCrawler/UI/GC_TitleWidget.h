// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_TitleWidget.generated.h"

class UTextBlock;
class UVerticalBox;

UCLASS()
class GRIDCRAWLER_API UGC_TitleWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetTitleStep(int32 Step);

protected:
	UFUNCTION()
	void OnLogoFadeInFinished();

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> LogoText;

	UPROPERTY()
	TObjectPtr<UTextBlock> PressSpaceText;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* LogoAnim;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* PressSpaceAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* PressSpaceAnim2;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_ExpTextWidget.generated.h"

class UTextBlock;

DECLARE_MULTICAST_DELEGATE(FOnExpTextFadeOutDelegate);

UCLASS()
class GRIDCRAWLER_API UGC_ExpTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	FOnExpTextFadeOutDelegate OnExpTextFadeOut;

	void SetExpText(int32 Exp);
	
	UFUNCTION()
	void ExpTextFadeOut();
	
protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> ExpText;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

	bool bAnimationPaused = false;

	float PausedTime;
};

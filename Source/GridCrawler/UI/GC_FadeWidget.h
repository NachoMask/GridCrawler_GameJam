// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_FadeWidget.generated.h"

class UImage;

DECLARE_MULTICAST_DELEGATE(FOnFadeOutDelegate);
DECLARE_MULTICAST_DELEGATE(FOnFadeInDelegate);

UCLASS()
class GRIDCRAWLER_API UGC_FadeWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	FOnFadeOutDelegate OnFadeOut;
	FOnFadeInDelegate OnFadeIn;

	void PlayFadeOut(float Time);

	void PlayFadeIn(float Time);

protected:
	UFUNCTION()
	void BroadcastFadeOut();

	UFUNCTION()
	void BroadcastFadeIn();

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeToBlack;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeFromBlack;
};

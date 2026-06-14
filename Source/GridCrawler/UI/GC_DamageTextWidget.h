// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_DamageTextWidget.generated.h"

class UTextBlock;

DECLARE_MULTICAST_DELEGATE(FOnDamageTextFadeOutDelegate);

UCLASS()
class GRIDCRAWLER_API UGC_DamageTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	FOnDamageTextFadeOutDelegate OnDamageTextFadeOut;

	void SetDamageText(int32 InDamageText);

	UFUNCTION()
	void DamageTextFadeOut();

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

	bool bAnimationPaused = false;

	float PauseTime;
};

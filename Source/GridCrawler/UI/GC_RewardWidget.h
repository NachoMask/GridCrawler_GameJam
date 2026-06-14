// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_RewardWidget.generated.h"

class UBorder;
class UTextBlock;
class UImage;

DECLARE_MULTICAST_DELEGATE(FOnRewardShowEndDelegate);

UCLASS()
class GRIDCRAWLER_API UGC_RewardWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	FOnRewardShowEndDelegate OnRewardShowEnd;

	void SetRewardTypeText(FText Text);
	void SetRewardIcon(UTexture2D* Icon);
	void SetRewardNameText(FText Text);
	void SetRewardSubText(FText Text);
	void SetRewardDescriptionText(FText Text);

	void SetRewardOutCursor();
	void SetRewardInCursor();

protected:
	UFUNCTION()
	void RewardShowEnd();

protected:
	UPROPERTY()
	TObjectPtr<UBorder> CursorBorder;

	UPROPERTY()
	TObjectPtr<UTextBlock> RewardTypeText;

	UPROPERTY()
	TObjectPtr<UImage> RewardIcon;

	UPROPERTY()
	TObjectPtr<UTextBlock> RewardNameText;

	UPROPERTY()
	TObjectPtr<UTextBlock> RewardSubText;

	UPROPERTY()
	TObjectPtr<UTextBlock> RewardDescriptionText;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ShowReward = nullptr;
};

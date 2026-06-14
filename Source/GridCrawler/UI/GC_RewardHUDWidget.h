// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GC_RewardHUDWidget.generated.h"

class UHorizontalBox;
class UGC_RewardWidget;

DECLARE_MULTICAST_DELEGATE(FOnAllRewardShowEnd);

UCLASS()
class GRIDCRAWLER_API UGC_RewardHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	FOnAllRewardShowEnd OnAllRewardShowEnd;

	void SetRewardRemove();

	void SetRewardHUD_Skill(UTexture2D* InSkillIcon, FName InSkillName, float InSkillCooldown, FName InSkillDesc);
	void SetRewardHUD_Stat(int32 StatEnumIndex, float InStatValue);

	void SetRewardHUDOutCursor(int32 Index);
	void SetRewardHUDInCursor(int32 Index);

	UFUNCTION()
	void CheckAllRewardShowEnd();

protected:
	void SetRewardHUDVisible(bool bVisible);

protected:
	UPROPERTY()
	TObjectPtr<UHorizontalBox> RewardHUDBox;

	UPROPERTY(EditAnywhere, Category = "GC|Reward")
	TSubclassOf<UGC_RewardWidget> RewardWidgetClass;

	UPROPERTY(EditAnywhere, Category = "GC|Reward")
	TArray<TObjectPtr<UGC_RewardWidget>> RewardWidgets;

	UPROPERTY(EditAnywhere, Category = "GC|Reward")
	TArray<FName> StatNameArr;

	int32 ShowEndRewardNum = 0;
};

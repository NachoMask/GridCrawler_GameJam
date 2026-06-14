// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_SkillWidget.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;

UCLASS()
class GRIDCRAWLER_API UGC_SkillWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetSkillKey(FText KeyText);
	void SetSkillData(UTexture2D* InSkillIcon);
	void SetSkillInUse();
	void SetSkillCooldown(float InCooldown);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SkillKey;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SkillIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> SkillCooldownBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SkillCooldownText;

	/*UPROPERTY()
	TObjectPtr<UTextBlock> SkillValue;*/

	UPROPERTY(VisibleAnywhere, Category = "GC|Cooldown")
	float MaxCooldown;

	UPROPERTY(VisibleAnywhere, Category = "GC|Cooldown")
	float Cooldown;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_SkillHUDWidget.generated.h"

class UGC_SkillWidget;
class UHorizontalBox;

UCLASS()
class GRIDCRAWLER_API UGC_SkillHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateSkillWidget(int32 SkillIndex, UTexture2D* InSkillIcon);
	void SetSkillInUse(int32 SkillIndex);
	void SetSkillCooldown(int32 SkillIndex, float CooldownTime);

protected:
	UPROPERTY()
	TObjectPtr<UHorizontalBox> SkillHUDBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGC_SkillWidget> SkillWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGC_SkillWidget> DashWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UGC_SkillWidget>> SkillWidgets;

	UPROPERTY(VisibleAnywhere, Category = "GC|Skill")
	int32 SkillNum = 4;

	UPROPERTY(EditAnywhere, Category = "GC|Skill")
	TArray<FText> SkillKeyArr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GC_UserWidget.h"
#include "GC_HpTextWidget.generated.h"

class UTextBlock;

UCLASS()
class GRIDCRAWLER_API UGC_HpTextWidget : public UGC_UserWidget
{
	GENERATED_BODY()
	
public:
	UGC_HpTextWidget();

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHpText(float CurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> HpText;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_TimerWidget.generated.h"

class UTextBlock;

UCLASS()
class GRIDCRAWLER_API UGC_TimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetTimerText(int32 TotalSeconds);

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> MinutesText;

	UPROPERTY()
	TObjectPtr<UTextBlock> SecondsText;
};

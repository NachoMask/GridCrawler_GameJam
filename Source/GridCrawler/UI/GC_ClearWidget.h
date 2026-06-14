// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_ClearWidget.generated.h"

class UBorder;
class UTextBlock;

UCLASS()
class GRIDCRAWLER_API UGC_ClearWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void MenuInit();
	void MenuOutCursor(int32 MenuIndex);
	void MenuInCursor(int32 MenuIndex);

protected:
	UPROPERTY()
	TObjectPtr<UBorder> MenuRestartCursor;

	UPROPERTY()
	TObjectPtr<UTextBlock> MenuRestartText;

	UPROPERTY()
	TObjectPtr<UBorder> MenuQuitCursor;

	UPROPERTY()
	TObjectPtr<UTextBlock> MenuQuitText;

	FLinearColor OutCursorColor;
	FLinearColor InCursorColor;
	FSlateColor OutCursorTextColor;
	FSlateColor InCursorTextColor;
};

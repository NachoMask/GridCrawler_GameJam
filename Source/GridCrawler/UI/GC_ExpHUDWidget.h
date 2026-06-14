// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_ExpHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class GRIDCRAWLER_API UGC_ExpHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetLevelText(int32 Level);
	void SetExpProgressBar(int32 CurrentExp, int32 MaxExp);

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY()
	TObjectPtr<UProgressBar> ExpProgressBar;
};

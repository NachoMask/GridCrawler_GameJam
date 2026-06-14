// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_LvExpWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class GRIDCRAWLER_API UGC_LvExpWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetExpBar(int32 CurrentExp, int32 MaxExp);
	void SetLvText(int32 Lv);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ExpBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> LvText;
};

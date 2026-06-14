// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GC_StatHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class GRIDCRAWLER_API UGC_StatHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetCurrentHpText(float CurrentHp);
	void SetMaxHpText(float MaxHp);
	void SetAttackText(float Attack);
	void SetArmorText(float Armor);
	void SetMoveSpeedText(float MoveSpeed);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CurrentHpText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MaxHpText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AttackText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ArmorText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MoveSpeedText;
};

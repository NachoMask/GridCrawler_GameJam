// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GC_TitleGameMode.generated.h"

class UGC_TitleWidget;

UCLASS()
class GRIDCRAWLER_API AGC_TitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GC|Widget")
	TSubclassOf<UGC_TitleWidget> TitleWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Widget")
	TObjectPtr<UGC_TitleWidget> TitleWidget = nullptr;
};

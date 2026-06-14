// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GC_TitlePlayerController.generated.h"

class UGC_TitleWidget;
class UGC_FadeWidget;

UCLASS()
class GRIDCRAWLER_API AGC_TitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnStartKeyPressed();

	/** Screen Transition */
	void FadeOut(float FadeTime);
	void FadeIn(float FadeTime);
	void OnFadeOutFinished();
	void OnFadeInFinished();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GC|Widget")
	TSubclassOf<UGC_TitleWidget> TitleWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Widget")
	TObjectPtr<UGC_TitleWidget> TitleWidget;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Widget")
	TSubclassOf<UGC_FadeWidget> FadeWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Widget")
	TObjectPtr<UGC_FadeWidget> FadeWidget;

	// 0: LogoAnim | 1: Title |
	UPROPERTY(VisibleAnywhere, Category = "GC")
	int32 CurrentStep = 0;	

private:
	bool bScreenTransitioning = false;
};

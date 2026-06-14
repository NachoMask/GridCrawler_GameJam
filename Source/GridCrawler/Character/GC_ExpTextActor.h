// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GC_ExpTextActor.generated.h"

class UWidgetComponent;
class UGC_ExpTextWidget;

UCLASS()
class GRIDCRAWLER_API AGC_ExpTextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGC_ExpTextActor();

public:
	void SetExpText(int32 Exp);

protected:
	UFUNCTION()
	void ExpTextFadeOut();

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "GC|Widget")
	TObjectPtr<UWidgetComponent> ExpText;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Widget")
	TSubclassOf<UGC_ExpTextWidget> ExpTextClass = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GC_DamageTextActor.generated.h"

class UWidgetComponent;
class UGC_DamageTextWidget;

UCLASS()
class GRIDCRAWLER_API AGC_DamageTextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGC_DamageTextActor();

public:
	void SetDamageText(float InDamageText);

protected:
	UFUNCTION()
	void DamageTextFadeOut();

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "GC|Widget")
	TObjectPtr<UWidgetComponent> DamageText;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Widget")
	TSubclassOf<UGC_DamageTextWidget> DamageTextClass = nullptr;
};

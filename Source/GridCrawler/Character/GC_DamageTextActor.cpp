// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC_DamageTextActor.h"

#include "Components\WidgetComponent.h"
#include "UI\GC_DamageTextWidget.h"

AGC_DamageTextActor::AGC_DamageTextActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	DamageText = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageText"));
	DamageText->SetupAttachment(GetRootComponent());
}

void AGC_DamageTextActor::SetDamageText(float InDamageText)
{
	if (DamageTextClass)
	{
		DamageText->SetWidgetClass(DamageTextClass);
		DamageText->SetWidgetSpace(EWidgetSpace::Screen);

		UGC_DamageTextWidget* DamageTextWidget = Cast<UGC_DamageTextWidget>(DamageText->GetUserWidgetObject());
		if (DamageTextWidget)
		{
			DamageTextWidget->SetDamageText(FMath::FloorToInt(InDamageText));
			DamageTextWidget->OnDamageTextFadeOut.AddUObject(this, &AGC_DamageTextActor::DamageTextFadeOut);
		}
	}
}

void AGC_DamageTextActor::DamageTextFadeOut()
{
	Destroy();
}

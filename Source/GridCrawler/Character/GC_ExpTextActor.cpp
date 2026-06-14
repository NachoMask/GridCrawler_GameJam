// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC_ExpTextActor.h"

#include "Components\WidgetComponent.h"
#include "UI\GC_ExpTextWidget.h"

AGC_ExpTextActor::AGC_ExpTextActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ExpText = CreateDefaultSubobject<UWidgetComponent>(TEXT("ExpText"));
	ExpText->SetupAttachment(GetRootComponent());
}

void AGC_ExpTextActor::SetExpText(int32 Exp)
{
	if (ExpText)
	{
		ExpText->SetWidgetClass(ExpTextClass);
		ExpText->SetWidgetSpace(EWidgetSpace::Screen);

		UGC_ExpTextWidget* ExpTextWidget = Cast<UGC_ExpTextWidget>(ExpText->GetUserWidgetObject());
		if (ExpTextWidget)
		{
			ExpTextWidget->SetExpText(Exp);
			ExpTextWidget->OnExpTextFadeOut.AddUObject(this, &AGC_ExpTextActor::ExpTextFadeOut);
		}
	}
}

void AGC_ExpTextActor::ExpTextFadeOut()
{
	Destroy();
}

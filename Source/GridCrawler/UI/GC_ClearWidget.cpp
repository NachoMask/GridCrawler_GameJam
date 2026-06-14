// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_ClearWidget.h"

#include "Components\Border.h"
#include "Components\TextBlock.h"

void UGC_ClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuRestartCursor = Cast<UBorder>(GetWidgetFromName(TEXT("MenuRestartCursor")));
	ensure(MenuRestartCursor);

	MenuRestartText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MenuRestartText")));
	ensure(MenuRestartText);

	MenuQuitCursor = Cast<UBorder>(GetWidgetFromName(TEXT("MenuQuitCursor")));
	ensure(MenuQuitCursor);
	
	MenuQuitText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MenuQuitText")));
	ensure(MenuQuitText);

	OutCursorColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.f);
	InCursorColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

	OutCursorTextColor = FLinearColor(0.f, 0.f, 0.f, 1.f);
	InCursorTextColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
}

void UGC_ClearWidget::MenuInit()
{
	MenuOutCursor(1);
	MenuOutCursor(2);
}

void UGC_ClearWidget::MenuOutCursor(int32 MenuIndex)
{
	switch (MenuIndex)
	{
	case 1:	// Menu : Restart
	{
		MenuRestartCursor->SetRenderScale(FVector2D(1.0f, 1.0f));
		MenuRestartCursor->SetBrushColor(OutCursorColor);
		MenuRestartText->SetColorAndOpacity(OutCursorTextColor);
		break;
	}
	case 2:	// Menu : Quit
	{
		MenuQuitCursor->SetRenderScale(FVector2D(1.0f, 1.0f));
		MenuQuitCursor->SetBrushColor(OutCursorColor);
		MenuQuitText->SetColorAndOpacity(OutCursorTextColor);
		break;
	}
	}
}

void UGC_ClearWidget::MenuInCursor(int32 MenuIndex)
{
	switch (MenuIndex)
	{
	case 1:	// Menu : Restart
	{
		MenuRestartCursor->SetRenderScale(FVector2D(1.0f, 1.0f));
		MenuRestartCursor->SetBrushColor(InCursorColor);
		MenuRestartText->SetColorAndOpacity(InCursorTextColor);
		break;
	}
	case 2:	// Menu : Quit
	{
		MenuQuitCursor->SetRenderScale(FVector2D(1.0f, 1.0f));
		MenuQuitCursor->SetBrushColor(InCursorColor);
		MenuQuitText->SetColorAndOpacity(InCursorTextColor);
		break;
	}
	}
}
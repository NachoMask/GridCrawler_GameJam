// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GC_PauseWidget.h"

#include "Components\Border.h"
#include "Components\TextBlock.h"

void UGC_PauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OutCursorColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.f);
	InCursorColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

	OutCursorTextColor = FLinearColor(0.f, 0.f, 0.f, 1.f);
	InCursorTextColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
}

void UGC_PauseWidget::MenuInit()
{
	MenuOutCursor(0);
	MenuOutCursor(1);
	MenuOutCursor(2);
}

void UGC_PauseWidget::MenuOutCursor(int32 MenuIndex)
{
	switch (MenuIndex)
	{
	case 0:	// Menu : Resume
	{
		MenuResumeCursor->SetRenderScale(FVector2D(1.0f, 1.0f));
		MenuResumeCursor->SetBrushColor(OutCursorColor);
		MenuResumeText->SetColorAndOpacity(OutCursorTextColor);
		break;
	}
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

void UGC_PauseWidget::MenuInCursor(int32 MenuIndex)
{
	switch (MenuIndex)
	{
	case 0:	// Menu : Resume
	{
		MenuResumeCursor->SetRenderScale(FVector2D(1.1f, 1.1f));
		MenuResumeCursor->SetBrushColor(InCursorColor);
		MenuResumeText->SetColorAndOpacity(InCursorTextColor);
		break;
	}
	case 1:	// Menu : Restart
	{
		MenuRestartCursor->SetRenderScale(FVector2D(1.1f, 1.1f));
		MenuRestartCursor->SetBrushColor(InCursorColor);
		MenuRestartText->SetColorAndOpacity(InCursorTextColor);
		break;
	}
	case 2:	// Menu : Quit
	{
		MenuQuitCursor->SetRenderScale(FVector2D(1.1f, 1.1f));
		MenuQuitCursor->SetBrushColor(InCursorColor);
		MenuQuitText->SetColorAndOpacity(InCursorTextColor);
		break;
	}
	}
}
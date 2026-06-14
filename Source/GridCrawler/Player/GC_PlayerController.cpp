// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GC_PlayerController.h"

#include "Character\GC_CharacterPlayer.h"
#include "Character\GC_CharacterStatComponent.h"
#include "Character\GC_SkillComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Kismet\GameplayStatics.h"
#include "Game\GC_GameMode.h"
#include "Game\GC_WaveManager.h"
#include "Game\GC_RewardManager.h"
#include "UI\GC_FadeWidget.h"
#include "UI\GC_HUDWidget.h"
#include "UI\GC_TimerWidget.h"
#include "UI\GC_StatHUDWidget.h"
#include "UI\GC_SkillHUDWidget.h"
#include "UI\GC_LvExpWidget.h"
#include "UI\GC_RewardHUDWidget.h"
#include "UI\GC_PauseWidget.h"
#include "UI\GC_ClearWidget.h"

void AGC_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	PossessedPlayer = Cast<AGC_CharacterPlayer>(aPawn);

	SetupInputs(BattleMappingContext);
}

void AGC_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->ClearActionBindings();

		BindActions_Pause(PauseInputAction);
		BindActions_Move(MoveInputAction);
		BindActions_Skill1(Skill1InputAction);
		BindActions_Skill2(Skill2InputAction);
		BindActions_Skill3(Skill3InputAction);
		BindActions_Skill4(Skill4InputAction);
		BindActions_Dash(DashInputAction);
		BindActions_RewardCursorMove(RewardCursorMoveAction);
		BindActions_RewardSelect(RewardSelectAction);
		BindActions_PauseCursorMove(PauseCursorMoveAction);
		BindActions_PauseMenuSelect(PauseMenuSelectAction);
	}
}

void AGC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	WaveManager = GetWorld()->GetAuthGameMode<AGC_GameMode>()->GetWaveManager();
	check(WaveManager);

	RewardManager = GetWorld()->GetAuthGameMode<AGC_GameMode>()->GetRewardManager();
	check(RewardManager);
	RewardManager->RegisterPlayerController(this);

	check(FadeWidgetClass);
	FadeWidget = CreateWidget<UGC_FadeWidget>(this, FadeWidgetClass);
	if (FadeWidget)
	{
		FadeWidget->AddToViewport(9999);
		FadeIn(1.f);
	}
}

void AGC_PlayerController::SetupInputs(UInputMappingContext* Context)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->ClearAllMappings();

		FModifyContextOptions Option;
		Option.bForceImmediately = true;

		Subsystem->AddMappingContext(Context, 0, Option);

		if (Context != PauseMappingContext)
		{
			PrePauseMappingContext = Context;
		}
	}
}

void AGC_PlayerController::ClearInputs()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
	}
}

void AGC_PlayerController::BindActions_Pause(UInputAction* InputAction)
{
	check(InputAction);

	InputAction->bTriggerWhenPaused = true;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction, ETriggerEvent::Started, this, &AGC_PlayerController::Toggle_Pause);
	}
}

void AGC_PlayerController::BindActions_Move(UInputAction* InputAction)
{
	check(InputAction);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::Move_Started);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Triggered,
			this,
			&AGC_PlayerController::Move_Triggered);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Completed,
			this,
			&AGC_PlayerController::Move_Completed);
	}
}

void AGC_PlayerController::BindActions_Skill1(UInputAction* InputAction)
{
	check(InputAction);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::Skill1_Started);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Triggered,
			this,
			&AGC_PlayerController::Skill1_Triggered);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Completed,
			this,
			&AGC_PlayerController::Skill1_Completed);
	}
}

void AGC_PlayerController::BindActions_Skill2(UInputAction* InputAction)
{
	check(Skill2InputAction);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			Skill2InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::Skill2_Started);

		EnhancedInputComponent->BindAction(
			Skill2InputAction,
			ETriggerEvent::Triggered,
			this,
			&AGC_PlayerController::Skill2_Triggered);

		EnhancedInputComponent->BindAction(
			Skill2InputAction,
			ETriggerEvent::Completed,
			this,
			&AGC_PlayerController::Skill2_Completed);
	}
}

void AGC_PlayerController::BindActions_Skill3(UInputAction* InputAction)
{
	check(InputAction);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::Skill3_Started);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Triggered,
			this,
			&AGC_PlayerController::Skill3_Triggered);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Completed,
			this,
			&AGC_PlayerController::Skill3_Completed);
	}
}

void AGC_PlayerController::BindActions_Skill4(UInputAction* InputAction)
{
	check(InputAction);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::Skill4_Started);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Triggered,
			this,
			&AGC_PlayerController::Skill4_Triggered);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Completed,
			this,
			&AGC_PlayerController::Skill4_Completed);
	}
}

void AGC_PlayerController::BindActions_Dash(UInputAction* InputAction)
{
	check(InputAction);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::Dash_Started);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Triggered,
			this,
			&AGC_PlayerController::Dash_Triggered);

		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Completed,
			this,
			&AGC_PlayerController::Dash_Completed);
	}
}

void AGC_PlayerController::BindActions_RewardCursorMove(UInputAction* InputAction)
{
	check(InputAction);

	InputAction->bTriggerWhenPaused = true;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::RewardCursorMove_Started);
	}
}

void AGC_PlayerController::BindActions_RewardSelect(UInputAction* InputAction)
{
	check(InputAction);

	InputAction->bTriggerWhenPaused = true;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::RewardSelect_Started);
	}
}

void AGC_PlayerController::BindActions_PauseCursorMove(UInputAction* InputAction)
{
	check(InputAction);

	InputAction->bTriggerWhenPaused = true;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::PauseCursorMove_Started);
	}
}

void AGC_PlayerController::BindActions_PauseMenuSelect(UInputAction* InputAction)
{
	check(InputAction);

	InputAction->bTriggerWhenPaused = true;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&AGC_PlayerController::PauseMenuSelect_Started);
	}
}

void AGC_PlayerController::BeginPause()
{
	if (bScreenTransitioning) return;

	UGameplayStatics::SetGamePaused(this, true);
}

void AGC_PlayerController::EndPause()
{
	if (bScreenTransitioning) return;

	UGameplayStatics::SetGamePaused(this, false);
}

void AGC_PlayerController::Toggle_Pause()
{
	if (bScreenTransitioning) return;

	if (!UGameplayStatics::IsGamePaused(this))
	{
		EnterPauseMode();
	}
	else
	{
		ExitPauseMode();
	}
	
}

void AGC_PlayerController::Move_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->MoveAction_Started(Value.Get<FVector2D>());
	}
}

void AGC_PlayerController::Move_Triggered(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->MoveAction_Triggered(Value.Get<FVector2D>());
	}
}

void AGC_PlayerController::Move_Completed(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->MoveAction_Completed(Value.Get<FVector2D>());
	}
}

void AGC_PlayerController::Skill1_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Started(0);
	}
}

void AGC_PlayerController::Skill1_Triggered(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Triggered(0);
	}
}

void AGC_PlayerController::Skill1_Completed(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Completed(0);
	}
}

void AGC_PlayerController::Skill2_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Started(1);
	}
}

void AGC_PlayerController::Skill2_Triggered(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Triggered(1);
	}
}

void AGC_PlayerController::Skill2_Completed(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Completed(1);
	}
}

void AGC_PlayerController::Skill3_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Started(2);
	}
}

void AGC_PlayerController::Skill3_Triggered(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Triggered(2);
	}
}

void AGC_PlayerController::Skill3_Completed(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Completed(2);
	}
}

void AGC_PlayerController::Skill4_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Started(3);
	}
}

void AGC_PlayerController::Skill4_Triggered(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Triggered(3);
	}
}

void AGC_PlayerController::Skill4_Completed(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Completed(3);
	}
}

void AGC_PlayerController::Dash_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Started(4);
	}
}

void AGC_PlayerController::Dash_Triggered(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Triggered(4);
	}
}

void AGC_PlayerController::Dash_Completed(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PossessedPlayer)
	{
		PossessedPlayer->SkillAction_Completed(4);
	}
}

void AGC_PlayerController::RewardCursorMove_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (RewardManager)
	{
		RewardManager->MoveRewardCursor(Value.GetMagnitude());
	}
}

void AGC_PlayerController::RewardSelect_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (RewardManager)
	{
		RewardManager->SelectReward();
	}
}

void AGC_PlayerController::PauseCursorMove_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	int32 MoveValue = Value.Get<float>();

	if (PlayerControlType == EPlayerControlType::Pause)
	{
		check(PauseWidget);

		PauseWidget->MenuOutCursor(PauseMenuCursor);

		PauseMenuCursor = FMath::Clamp(PauseMenuCursor + MoveValue, 0, PauseMenuNum - 1);

		PauseWidget->MenuInCursor(PauseMenuCursor);
	}
	else if (PlayerControlType == EPlayerControlType::Clear)
	{
		check(ClearWidget);

		ClearWidget->MenuOutCursor(ClearMenuCursor);

		ClearMenuCursor = FMath::Clamp(ClearMenuCursor + MoveValue, 1, PauseMenuNum);

		ClearWidget->MenuInCursor(ClearMenuCursor);
	}
}

void AGC_PlayerController::PauseMenuSelect_Started(const FInputActionValue& Value)
{
	if (bScreenTransitioning) return;

	if (PlayerControlType == EPlayerControlType::Pause)
	{
		switch (PauseMenuCursor)
		{
		case 0:	// Menu : Resume
		{
			Toggle_Pause();
			break;
		}
		case 1:	// Menu : Restart
		{
			FadeOut(1.f);
			// OnFadeOutFinished();
			break;
		}
		case 2:	// Menu : Quit
		{
			UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
			break;
		}
		}
	}
	else if (PlayerControlType == EPlayerControlType::Clear)
	{
		switch (ClearMenuCursor)
		{
		case 1:	// Menu : Restart
		{
			FadeOut(1.f);
			// OnFadeOutFinished();
			break;
		}
		case 2:	// Menu : Quit
		{
			UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
			break;
		}
		}
	}
}

void AGC_PlayerController::FadeOut(float FadeTime)
{
	UE_LOG(LogTemp, Warning, TEXT("FadeOut"));

	ClearInputs();

	bScreenTransitioning = true;

	if (FadeWidget)
	{
		FadeWidget->PlayFadeOut(FadeTime);
	}

	// In Paused, Timer Not Working
	/*FTimerHandle RestartHandle;
	GetWorld()->GetTimerManager().SetTimer(
		RestartHandle, this, &AGC_PlayerController::OnFadeOutFinished, FadeTime, false);*/
}

void AGC_PlayerController::FadeIn(float FadeTime)
{
	bScreenTransitioning = true;

	if (FadeWidget)
	{
		FadeWidget->PlayFadeIn(FadeTime);
	}

	FTimerHandle RestartHandle;
	GetWorld()->GetTimerManager().SetTimer(
		RestartHandle, this, &AGC_PlayerController::OnFadeInFinished, FadeTime, false);
}

void AGC_PlayerController::OnFadeOutFinished()
{
	if (bRestarting) return;

	bRestarting = true;

	UE_LOG(LogTemp, Warning, TEXT("OnFadeOutFinished"));

	// GetWorldTimerManager().ClearAllTimersForObject(PossessedPlayer);

	// UGameplayStatics::SetGamePaused(this, false);

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));

	GetWorldTimerManager().SetTimerForNextTick(
		[&]()
		{
			// GetWorld()->GetFirstPlayerController()->RestartLevel();
			
		});
}

void AGC_PlayerController::OnFadeInFinished()
{
	bScreenTransitioning = false;

	check(HUDWidgetClass);
	HUDWidget = CreateWidget<UGC_HUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport(0);
	}

	check(RewardHUDWidgetClass);
	RewardHUDWidget = CreateWidget<UGC_RewardHUDWidget>(this, RewardHUDWidgetClass);
	if (RewardHUDWidget)
	{
		RewardHUDWidget->AddToViewport(1);
		RewardHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	check(StatHUDWidgetClass);
	StatHUDWidget = CreateWidget<UGC_StatHUDWidget>(this, StatHUDWidgetClass);
	if (StatHUDWidget)
	{
		StatHUDWidget->AddToViewport(9998);
		StatHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	// Request Player's Skill Datas And Init SkillHUD
	BindPlayerComponentToWidget();

	PossessedPlayer->RequestBroadcastStats();
	PossessedPlayer->RequestBroadcastSkills();

	check(PauseWidgetClass);
	PauseWidget = CreateWidget<UGC_PauseWidget>(this, PauseWidgetClass);
	if (PauseWidget)
	{
		PauseWidget->AddToViewport(9997);
		PauseWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	//RewardManager->GenerateStartSkill(3);

	ShowTutorialWidget();
	// WaveManager->StartWave();
	WaveManager->RequestWaveStart();
}

void AGC_PlayerController::BindPlayerComponentToWidget()
{
	check(HUDWidget);
	check(RewardHUDWidget);
	check(StatHUDWidget);
	check(FadeWidget);

	UGC_TimerWidget* TimerWidget = HUDWidget->GetTimerWidget();
	UGC_LvExpWidget* LvExpWidget = HUDWidget->GetLvExpWidget();
	UGC_SkillHUDWidget* SkillHUDWidget = HUDWidget->GetSkillHUDWidget();

	check(WaveManager);
	check(TimerWidget);
	check(LvExpWidget);
	check(SkillHUDWidget);
	
	/** Fade */
	FadeWidget->OnFadeIn.AddUObject(this, &AGC_PlayerController::OnFadeInFinished);
	FadeWidget->OnFadeOut.AddUObject(this, &AGC_PlayerController::OnFadeOutFinished);

	/** Timer */
	WaveManager->OnTimeChange.AddUObject(TimerWidget, &UGC_TimerWidget::SetTimerText);
	WaveManager->OnGameClear.AddUObject(this, &AGC_PlayerController::EnterClearMode);

	/** LvExp */
	PossessedPlayer->OnLevelUp.AddUObject(LvExpWidget, &UGC_LvExpWidget::SetLvText);
	PossessedPlayer->OnExpChange.AddUObject(LvExpWidget, &UGC_LvExpWidget::SetExpBar);

	/** Stat */
	PossessedPlayer->GetStatComponent()->
		OnHpChanged.AddUObject(StatHUDWidget, &UGC_StatHUDWidget::SetCurrentHpText);
	PossessedPlayer->GetStatComponent()->
		OnMaxHpChanged.AddUObject(StatHUDWidget, &UGC_StatHUDWidget::SetMaxHpText);
	PossessedPlayer->GetStatComponent()->
		OnAttackChanged.AddUObject(StatHUDWidget, &UGC_StatHUDWidget::SetAttackText);
	PossessedPlayer->GetStatComponent()->
		OnArmorChanged.AddUObject(StatHUDWidget, &UGC_StatHUDWidget::SetArmorText);
	PossessedPlayer->GetStatComponent()->
		OnMoveSpeedChanged.AddUObject(StatHUDWidget, &UGC_StatHUDWidget::SetMoveSpeedText);

	/** Skill HUD */
	PossessedPlayer->GetSkillComponent()->
		OnSkillChange.AddUObject(SkillHUDWidget, &UGC_SkillHUDWidget::UpdateSkillWidget);
	PossessedPlayer->GetSkillComponent()->
		OnSkillInUse.AddUObject(SkillHUDWidget, &UGC_SkillHUDWidget::SetSkillInUse);
	PossessedPlayer->GetSkillComponent()->
		OnSkillCooldown.AddUObject(SkillHUDWidget, &UGC_SkillHUDWidget::SetSkillCooldown);

	PossessedPlayer->GetStatComponent()->
		OnAttackChanged.AddUObject(PossessedPlayer->GetSkillComponent(), &UGC_SkillComponent::BroadcastCurrentSkills);

	/** Reward HUD */
	RewardManager->
		OnSkillReward.AddUObject(RewardHUDWidget, &UGC_RewardHUDWidget::SetRewardHUD_Skill);
	RewardManager->
		OnStatReward.AddUObject(RewardHUDWidget, &UGC_RewardHUDWidget::SetRewardHUD_Stat);
	RewardManager->
		OnRewardOutCursor.AddUObject(RewardHUDWidget, &UGC_RewardHUDWidget::SetRewardHUDOutCursor);
	RewardManager->
		OnRewardInCursor.AddUObject(RewardHUDWidget, &UGC_RewardHUDWidget::SetRewardHUDInCursor);
	RewardManager->
		OnRewardRemove.AddUObject(RewardHUDWidget, &UGC_RewardHUDWidget::SetRewardRemove);

	RewardHUDWidget->OnAllRewardShowEnd.AddUObject(this, &AGC_PlayerController::EnableRewardModeInput);
}

void AGC_PlayerController::ShowTutorialWidget()
{
	check(TutorialWidgetClass);
	TutorialWidget = CreateWidget(this, TutorialWidgetClass);
	if (TutorialWidget)
	{
		TutorialWidget->AddToViewport();
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGC_PlayerController::HideTutorialWidget, 5.f);
}

void AGC_PlayerController::HideTutorialWidget()
{
	TutorialWidget->RemoveFromParent();
}

void AGC_PlayerController::RequestReward()
{
	RewardManager->GenerateReward(3);
}

void AGC_PlayerController::EnterRewardMode()
{
	PlayerControlType = EPlayerControlType::Reward;

	BeginPause();

	if (RewardHUDWidget)
	{
		RewardHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (StatHUDWidget)
	{
		StatHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AGC_PlayerController::ExitRewardMode()
{
	PlayerControlType = EPlayerControlType::Battle;

	SetupInputs(BattleMappingContext);

	if (RewardHUDWidget)
	{
		RewardHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (StatHUDWidget)
	{
		StatHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	EndPause();
}

void AGC_PlayerController::EnterPauseMode()
{
	PlayerControlType = EPlayerControlType::Pause;

	BeginPause();

	PauseMenuCursor = -1;
	PauseWidget->MenuInit();
	PauseWidget->SetVisibility(ESlateVisibility::Visible);

	if (StatHUDWidget)
	{
		StatHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}

	SetupInputs(PauseMappingContext);
}

void AGC_PlayerController::ExitPauseMode()
{
	PlayerControlType = EPlayerControlType::Battle;

	SetupInputs(PrePauseMappingContext);

	PauseWidget->SetVisibility(ESlateVisibility::Hidden);

	if (StatHUDWidget)
	{
		StatHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	EndPause();
}

void AGC_PlayerController::EnterDeathMode(float FadeTime)
{
	PlayerControlType = EPlayerControlType::Death;

	ClearInputs();

	UE_LOG(LogTemp, Warning, TEXT("Player Death"));

	FadeOut(FadeTime);
}

void AGC_PlayerController::EnterClearMode()
{
	PlayerControlType = EPlayerControlType::Clear;

	BeginPause();

	ClearWidget = CreateWidget<UGC_ClearWidget>(this, ClearWidgetClass);
	check(ClearWidget);
	ClearWidget->AddToViewport(9999);

	ClearMenuCursor = -1;
	ClearWidget->MenuInit();
	ClearWidget->SetVisibility(ESlateVisibility::Visible);

	SetupInputs(PauseMappingContext);
}

void AGC_PlayerController::EnableRewardModeInput()
{
	SetupInputs(RewardMappingContext);
}
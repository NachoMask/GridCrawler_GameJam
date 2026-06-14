// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GC_PlayerController.generated.h"

class AGC_CharacterPlayer;
class UInputAction;
class UInputMappingContext;
class AGC_WaveManager;
class AGC_RewardManager;
class UGC_FadeWidget;
class UGC_HUDWidget;
class UGC_StatHUDWidget;
class UGC_RewardHUDWidget;
class UGC_PauseWidget;
class UGC_ClearWidget;

UENUM()
enum class EPlayerControlType : uint8
{
	Battle,
	Reward,
	Pause,
	Death,
	Clear
};

UCLASS()
class GRIDCRAWLER_API AGC_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

protected:
	void SetupInputs(UInputMappingContext* Context);
	void ClearInputs();

	void BindActions_Pause(UInputAction* InputAction);

	/** Bind Actions <Battle> */
	void BindActions_Move(UInputAction* InputAction);
	void BindActions_Skill1(UInputAction* InputAction);
	void BindActions_Skill2(UInputAction* InputAction);
	void BindActions_Skill3(UInputAction* InputAction);
	void BindActions_Skill4(UInputAction* InputAction);
	void BindActions_Dash(UInputAction* InputAction);

	/** Bind Actions <Reward> */
	void BindActions_RewardCursorMove(UInputAction* InputAction);
	void BindActions_RewardSelect(UInputAction* InputAction);

	/** Bind Actions <Pause >*/
	void BindActions_PauseCursorMove(UInputAction* InputAction);
	void BindActions_PauseMenuSelect(UInputAction* InputAction);
	
	void BeginPause();
	void EndPause();
	void Toggle_Pause();

	UFUNCTION()
	void Move_Started(const FInputActionValue& Value);

	UFUNCTION()
	void Move_Triggered(const FInputActionValue& Value);

	UFUNCTION()
	void Move_Completed(const FInputActionValue& Value);

	UFUNCTION()
	void Skill1_Started(const FInputActionValue& Value);

	UFUNCTION()
	void Skill1_Triggered(const FInputActionValue& Value);

	UFUNCTION()
	void Skill1_Completed(const FInputActionValue& Value);

	UFUNCTION()
	void Skill2_Started(const FInputActionValue& Value);

	UFUNCTION()
	void Skill2_Triggered(const FInputActionValue& Value);

	UFUNCTION()
	void Skill2_Completed(const FInputActionValue& Value);

	UFUNCTION()
	void Skill3_Started(const FInputActionValue& Value);

	UFUNCTION()
	void Skill3_Triggered(const FInputActionValue& Value);

	UFUNCTION()
	void Skill3_Completed(const FInputActionValue& Value);

	UFUNCTION()
	void Skill4_Started(const FInputActionValue& Value);

	UFUNCTION()
	void Skill4_Triggered(const FInputActionValue& Value);

	UFUNCTION()
	void Skill4_Completed(const FInputActionValue& Value);

	UFUNCTION()
	void Dash_Started(const FInputActionValue& Value);

	UFUNCTION()
	void Dash_Triggered(const FInputActionValue& Value);

	UFUNCTION()
	void Dash_Completed(const FInputActionValue& Value);

	UFUNCTION()
	void RewardCursorMove_Started(const FInputActionValue& Value);

	UFUNCTION()
	void RewardSelect_Started(const FInputActionValue& Value);

	UFUNCTION()
	void PauseCursorMove_Started(const FInputActionValue& Value);

	UFUNCTION()
	void PauseMenuSelect_Started(const FInputActionValue& Value);

	/** Screen Transition */
	void FadeOut(float FadeTime);
	void FadeIn(float FadeTime);
	
	UFUNCTION()
	void OnFadeOutFinished();
	UFUNCTION()
	void OnFadeInFinished();

public:
	/** HUD */
	/** SkillHUD */
	void BindPlayerComponentToWidget();

	void ShowTutorialWidget();
	void HideTutorialWidget();

	void RequestReward();

	void EnterRewardMode();
	void ExitRewardMode();

	void EnterPauseMode();
	void ExitPauseMode();

	void EnterDeathMode(float FadeTime);

	void EnterClearMode();

	EPlayerControlType GetPlayerControlType() const
	{
		return PlayerControlType;
	}

protected:
	UFUNCTION()
	void EnableRewardModeInput();

protected:
	UPROPERTY(VisibleAnywhere, Category = "GC")
	TObjectPtr<AGC_CharacterPlayer> PossessedPlayer = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC")
	TObjectPtr<AGC_WaveManager> WaveManager = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC")
	TObjectPtr<AGC_RewardManager> RewardManager = nullptr;

	/** ControlType */
	UPROPERTY(VisibleAnywhere, Category = "GC|Input")
	EPlayerControlType PlayerControlType = EPlayerControlType::Battle;

	/** Input Mapping Context */
	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputMappingContext> BattleMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputMappingContext> RewardMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputMappingContext> PauseMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputMappingContext> ClearMappingContext = nullptr;

	/** Input Action */
	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> PauseInputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> MoveInputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> Skill1InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> Skill2InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> Skill3InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> Skill4InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> DashInputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> RewardCursorMoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> RewardSelectAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> PauseCursorMoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> PauseMenuSelectAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> ClearCursorMoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Input")
	TObjectPtr<UInputAction> ClearMenuSelectAction = nullptr;

	/** Widget */
	UPROPERTY(EditDefaultsOnly, Category = "GC|HUD")
	TSubclassOf<UGC_FadeWidget> FadeWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|HUD")
	TObjectPtr<UGC_FadeWidget> FadeWidget;

	UPROPERTY(EditAnywhere, Category = "GC|HUD")
	TSubclassOf<UGC_HUDWidget> HUDWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|HUD")
	TObjectPtr<UGC_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "GC|HUD")
	TSubclassOf<UUserWidget> TutorialWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|HUD")
	TObjectPtr<UUserWidget> TutorialWidget;

	UPROPERTY(VisibleAnywhere, Category = "GC|HUD")
	TObjectPtr<UGC_StatHUDWidget> StatHUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "GC|HUD")
	TSubclassOf<UGC_StatHUDWidget> StatHUDWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|HUD")
	TSubclassOf<UGC_RewardHUDWidget> RewardHUDWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|HUD")
	TObjectPtr<UGC_RewardHUDWidget> RewardHUDWidget;

	UPROPERTY(EditAnywhere, Category = "GC|HUD")
	TSubclassOf<UGC_PauseWidget> PauseWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|HUD")
	TObjectPtr<UGC_PauseWidget> PauseWidget;

	UPROPERTY(EditAnywhere, Category = "GC|HUD")
	TSubclassOf<UGC_ClearWidget> ClearWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|HUD")
	TObjectPtr<UGC_ClearWidget> ClearWidget;

	UPROPERTY(VisibleAnywhere, Category = "GC|Pause")
	bool bScreenTransitioning = false;

	/** Pause */
	UPROPERTY(VisibleAnywhere, Category = "GC|Pause")
	int32 PauseMenuCursor = -1;

	UPROPERTY(VisibleAnywhere, Category = "GC|Pause")
	int32 PauseMenuNum = 3;

	UPROPERTY(VisibleAnywhere, Category = "GC|Pause")
	TObjectPtr<UInputMappingContext> PrePauseMappingContext = nullptr;

	/** Clear */
	UPROPERTY(VisibleAnywhere, Category = "GC|Pause")
	int32 ClearMenuCursor = -1;

	UPROPERTY(VisibleAnywhere, Category = "GC|Pause")
	int32 ClearMenuNum = 2;

	/** Restart */
	UPROPERTY(VisibleAnywhere, Category = "GC|Restart")
	bool bRestarting = false;
};

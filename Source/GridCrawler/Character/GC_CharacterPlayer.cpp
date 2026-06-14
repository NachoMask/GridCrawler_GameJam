// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC_CharacterPlayer.h"

#include "Player\GC_PlayerController.h"
#include "Game\GC_GridManager.h"
#include "Game\GC_BattleManager.h"
#include "Character\GC_CharacterStatComponent.h"
#include "Character\GC_SkillComponent.h"
#include "Skill\GC_SkillData.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Components\WidgetComponent.h"
#include "UI\GC_HpTextWidget.h"

AGC_CharacterPlayer::AGC_CharacterPlayer()
{
	HpText = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPTEXT"));
	HpText->SetupAttachment(GetMesh());
}

void AGC_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AGC_CharacterPlayer::PossessedBy(AController* NewController)
{
	PlayerController = Cast<AGC_PlayerController>(NewController);
}

void AGC_CharacterPlayer::Init(AGC_GridManager* InGridManager, AGC_BattleManager* InBattleManager, const FGridCoord& Coord)
{
	Super::Init(InGridManager, InBattleManager, Coord);

	check(GridManager);

	LeftHandMesh->AttachToComponent(
		GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_lSocket"));
	RightHandMesh->AttachToComponent(
		GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));

	// Build Pathfind Field
	GridManager->RebuildPathfindField(GetGridCoord());

	if (HpTextClass != nullptr)
	{
		HpText->SetWidgetClass(HpTextClass);
		HpText->SetDrawSize(FVector2D(200.f, -30.f));
		HpText->SetWidgetSpace(EWidgetSpace::Screen);

		UGC_HpTextWidget* HpTextWidget = Cast<UGC_HpTextWidget>(HpText->GetUserWidgetObject());
		if (HpTextWidget)
		{
			HpTextWidget->UpdateHpText(GetStatCurrentHp());
			StatComp->OnHpChanged.AddUObject(HpTextWidget, &UGC_HpTextWidget::UpdateHpText);
		}
	}

	// Set Team
	SetTeam(ETeam::Player);

	// Initialize Stats
	StatComp->InitStat(PlayerStartStats);

	// Initialize Start Dash Skill
	SkillComp->InitSkills(StartSkills);
	SkillComp->ChangeSkill(SkillComp->GetMaxSkillNum(), StartDashSkill);
}

void AGC_CharacterPlayer::MoveAction_Started(FVector2D Value)
{
	SetWantsAim(true);
	SetWantsMove(true);

	// Clear AimingTimer
	GetWorld()->GetTimerManager().ClearTimer(AimingTimer);

	// Set WantsDirection
	SetWantsDirection(InputValueToDirection(Value));
		
	DecisionAction();
}

void AGC_CharacterPlayer::MoveAction_Triggered(FVector2D Value)
{
	SetWantsAim(true);
	SetWantsMove(true);

	// Set WantsDirection
	SetWantsDirection(InputValueToDirection(Value));

	if (GetCharacterState() == ECharacterState::Idle)
	{
		DecisionAction();
	}
}

void AGC_CharacterPlayer::MoveAction_Completed(FVector2D Value)
{
	// SetWantsAim(false);
	SetWantsMove(false);
}

void AGC_CharacterPlayer::SkillAction_Started(int32 Idx)
{
	if (SkillComp->GetSkillData(Idx))
	{
		SetWantsSkill(true);
		SetWantsSkillIndex(Idx);

		DecisionAction();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO Skill %d"), Idx + 1);
	}
}

void AGC_CharacterPlayer::SkillAction_Triggered(int32 Idx)
{
	if (SkillComp->GetSkillData(Idx))
	{
	}
}

void AGC_CharacterPlayer::SkillAction_Completed(int32 Idx)
{
	if (SkillComp->GetSkillData(Idx))
	{
	}
}

int32 AGC_CharacterPlayer::GetEmptySkillSlot() const
{
	return SkillComp->GetNextEmptySkillSlot();
}

void AGC_CharacterPlayer::ChangeSkill(const UGC_SkillData* Skill)
{
	int32 EmptyIndex = SkillComp->GetNextEmptySkillSlot();

	SkillComp->ChangeSkill(EmptyIndex, Skill);
}

void AGC_CharacterPlayer::RequestBroadcastStats()
{
	OnLevelUp.Broadcast(Level);
	OnExpChange.Broadcast(CurrentExp, MaxExp);
	StatComp->BroadcastCurrentStats();
}

void AGC_CharacterPlayer::RequestBroadcastSkills()
{
	SkillComp->BroadcastCurrentSkills();
}

void AGC_CharacterPlayer::AddExp(int32 Amount)
{
	CurrentExp += Amount;
	OnExpChange.Broadcast(CurrentExp, MaxExp);

	if (PlayerController->GetPlayerControlType() == EPlayerControlType::Battle)
	{
		CheckExp();
	}
}

void AGC_CharacterPlayer::CheckExp()
{
	if (CurrentExp >= MaxExp)
	{
		CurrentExp -= MaxExp;
		LevelUp();
	}
}

void AGC_CharacterPlayer::TryMove(ECharacterDirection InDirection)
{
	Super::TryMove(InDirection);

	OnPlayerMove.Broadcast(GetGridCoord());
}

void AGC_CharacterPlayer::EndSkill(UAnimMontage* AnimMontage, bool bInterrupted)
{
	Super::EndSkill(AnimMontage, bInterrupted);

	LeftHandMesh->SetHiddenInGame(true);
	RightHandMesh->SetHiddenInGame(true);
}

void AGC_CharacterPlayer::TryForcedMove()
{
	Super::TryForcedMove();

	OnPlayerMove.Broadcast(GetGridCoord());
}

ECharacterDirection AGC_CharacterPlayer::InputValueToDirection(FVector2D Value)
{
	ECharacterDirection InputDirection = ECharacterDirection::None;

	if (Value.Y != 0)
	{
		InputDirection = Value.Y > 0 ? ECharacterDirection::Right : ECharacterDirection::Left;
	}
	else if (Value.X != 0)
	{
		InputDirection = Value.X > 0 ? ECharacterDirection::Up : ECharacterDirection::Down;
	}

	return InputDirection;
}

void AGC_CharacterPlayer::LevelUp()
{
	++Level;
	OnLevelUp.Broadcast(Level);

	MaxExp = 1 + (Level - 1) * 3;
	OnExpChange.Broadcast(CurrentExp, MaxExp);

	// Request Reward
	PlayerController->RequestReward();
}

void AGC_CharacterPlayer::DeathCharacter()
{
	Super::DeathCharacter();

	PlayerController->EnterDeathMode(DeathMontage->GetPlayLength());

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
}

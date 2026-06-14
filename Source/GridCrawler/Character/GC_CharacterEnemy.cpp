// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC_CharacterEnemy.h"

#include "Game\GC_GridManager.h"
#include "Game\GC_BattleManager.h"
#include "Animation\GC_AnimInstance.h"
#include "Enemy\GC_EnemyData.h"
#include "Character\GC_CharacterStatComponent.h"
#include "Character\GC_SkillComponent.h"
#include "Skill\GC_SkillData.h"
#include "Components\WidgetComponent.h"
#include "UI\GC_HpTextWidget.h"
#include "Character\GC_ExpTextActor.h"

AGC_CharacterEnemy::AGC_CharacterEnemy()
{
	HpText = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPTEXT"));
	HpText->SetupAttachment(GetMesh());
}

void AGC_CharacterEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Data) return;
	if (bIsDead) return;

	if (CharacterState != ECharacterState::Skill)
	{
		DecisionTimer += DeltaSeconds;

		if (DecisionTimer >= Data->DecisionInterval)
		{
			TArray<int32> SkillCandidates;
			float CurrentTime = GetWorld()->GetTimeSeconds();

			for (int32 i = 0; i < Data->Skills.Num(); ++i)
			{
				if (SkillComp->IsSkillInCooldown(i, CurrentTime)) continue;
				if (BattleManager->IsTargetInRange(this, Data->Skills[i]) == false) continue;

				SetWantsSkill(true);
				SkillCandidates.Add(i);
			}

			if (!GetWantsSkill() || SkillCandidates.Num() <= 0)
			{
				SetWantsAim(true);
				SetWantsMove(true);

				ECharacterDirection MoveDir
					= GridManager->PathfindToPlayer(GetGridCoord(), GetPrevCoord());
				
				if (MoveDir != ECharacterDirection::None)
				{
					SetWantsDirection(MoveDir);
				}
				else
				{
					SetWantsDirection(GridManager->GetDirToPlayer(GetGridCoord()));
				}
			}
			else
			{
				int32 Index = FMath::RandRange(0, SkillCandidates.Num() - 1);

				SetWantsSkillIndex(SkillCandidates[Index]);
			}

			DecisionAction();

			DecisionTimer = 0.f;
		}
	}
}

void AGC_CharacterEnemy::Init(AGC_GridManager* InGridManager, AGC_BattleManager* InBattleManager, const FGridCoord& Coord)
{
	Super::Init(InGridManager, InBattleManager, Coord);

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
	SetTeam(ETeam::Enemy);
}

void AGC_CharacterEnemy::ApplyEnemyData(UGC_EnemyData* InData)
{
	Data = InData;

	GetMesh()->SetSkeletalMesh(InData->Mesh);
	GetMesh()->SetAnimInstanceClass(InData->AnimClass);

	LeftHandMesh->AttachToComponent(
		GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_lSocket"));
	RightHandMesh->AttachToComponent(
		GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));

	LeftHandMesh->SetStaticMesh(Data->LeftHandMesh);
	RightHandMesh->SetStaticMesh(Data->RightHandMesh);
	LeftHandMesh->SetRelativeRotation(Data->LeftHandMeshRotator);
	RightHandMesh->SetRelativeRotation(Data->RightHandMeshRotator);

	// Initialize Stat
	StatComp->InitStat(InData->Stats);

	// SkillComp->ApplySkills()
	SkillComp->InitSkills(InData->Skills);
}

int32 AGC_CharacterEnemy::GetEnemyExp() const
{
	if (Data)
	{
		return Data->Exp;
	}

	return 0;
}

void AGC_CharacterEnemy::RequestMove(ECharacterDirection MoveDir)
{
	if (MoveDir != ECharacterDirection::None)
		UpdateCharacterDirection(MoveDir);
	else
		UpdateCharacterDirection(GridManager->GetDirToPlayer(GetGridCoord()));

	TryMove(MoveDir);
}

void AGC_CharacterEnemy::TryMove(ECharacterDirection InDirection)
{
	if (bIsDead) return;

	PrevCoord = GetGridCoord();

	Super::TryMove(InDirection);
}

void AGC_CharacterEnemy::EndSkill(UAnimMontage* AnimMontage, bool bInterrupted)
{
	Super::EndSkill(AnimMontage, bInterrupted);

	LeftHandMesh->SetRelativeRotation(Data->LeftHandMeshRotator);
	RightHandMesh->SetRelativeRotation(Data->RightHandMeshRotator);
}

void AGC_CharacterEnemy::DeathCharacter()
{
	Super::DeathCharacter();

	bIsDead = true;

	GridManager->ClearOccupyingActor(GetGridCoord());

	HpText->SetVisibility(false);

	UGC_AnimInstance* AnimInstance = Cast<UGC_AnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Data->DeathMontage, 1.f);
		AnimInstance->SetActorDeath();
		
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AGC_CharacterEnemy::DeathProcess);
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, Data->DeathMontage);

		SetActorEnableCollision(false);
	}
}

void AGC_CharacterEnemy::DeathProcess(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (GetEnemyExp() > 0)
	{ 
		check(ExpTextActorClass);

		// Spawn Exp Gain Widget Actor
		FVector Location = GridManager->CoordToLocation(GetGridCoord());
		AGC_ExpTextActor* ExpText
			= GetWorld()->SpawnActor<AGC_ExpTextActor>(ExpTextActorClass, Location, FRotator::ZeroRotator);
		ExpText->SetExpText(GetEnemyExp());
	}

	OnEnemyDeath.Broadcast(this);
	Destroy();
}

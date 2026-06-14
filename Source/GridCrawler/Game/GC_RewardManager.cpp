// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GC_RewardManager.h"

#include "Game\GC_StageManager.h"
#include "Character\GC_CharacterPlayer.h"
#include "Player\GC_PlayerController.h"
#include "Character\GC_CharacterStatComponent.h"
#include "Algo\RandomShuffle.h"
#include "Skill\GC_SkillData.h"

AGC_RewardManager::AGC_RewardManager()
{

}

void AGC_RewardManager::Init(AGC_CharacterPlayer* InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;
}

void AGC_RewardManager::RegisterPlayerController(AGC_PlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}

void AGC_RewardManager::GenerateStartSkill(int32 Count)
{
	TArray<FRewardData> ShufflePool;
	for (FRewardData RewardData : SkillRewardPool)
	{
		if (RewardData.RewardType == ERewardType::Skill &&
			!SelectedRewardID.Contains(RewardData.SkillData->SkillName))
		{
			ShufflePool.Add(RewardData);
		}
	}
	Algo::RandomShuffle(ShufflePool);

	TArray<FRewardData> Result;

	for (int32 i = 0; i < Count; ++i)
	{
		if (ShufflePool.IsValidIndex(i))
		{
			const FRewardData& Reward = ShufflePool[i];

			if (Reward.RewardType == ERewardType::Skill)
			{
				Result.Add(Reward);

				UTexture2D* SkillIcon = Reward.SkillData->SkillIcon;
				FName SkillName = Reward.SkillData->SkillName;
				float SkillCooldown = Reward.SkillData->SkillCooldown;
				FName SkillDesc = Reward.SkillData->SkillDescription;

				OnSkillReward.Broadcast(SkillIcon, SkillName, SkillCooldown, SkillDesc);
			}
		}
	}

	CurrentReward = Result;

	if (PlayerController)
	{
		PlayerController->EnterRewardMode();

		if (CurrentReward.Num() <= 0)
		{
			PlayerController->ExitRewardMode();
			RemoveReward();
		}
	}
}

void AGC_RewardManager::GenerateReward(int32 Count)
{
	TArray<FRewardData> ShuffledSkillPool;

	for (FRewardData RewardData : SkillRewardPool)
	{
		if (RewardData.RewardType != ERewardType::Skill ||
			!SelectedRewardID.Contains(RewardData.SkillData->SkillName))
		{
			ShuffledSkillPool.Add(RewardData);
		}
	}
	Algo::RandomShuffle(ShuffledSkillPool);

	TArray<FRewardData> Result;
	int32 RewardNum = Count;

	if (PlayerCharacter->GetEmptySkillSlot() != -1)
	{
		int32 SkillRewardNum = FMath::RandRange(0, 3);

		for (int32 i = 0; i < SkillRewardNum; ++i)
		{
			if (ShuffledSkillPool.IsValidIndex(i))
			{
				const FRewardData& Reward = ShuffledSkillPool[i];

				if (Reward.RewardType == ERewardType::Skill)
				{
					UTexture2D* SkillIcon = Reward.SkillData->SkillIcon;
					FName SkillName = Reward.SkillData->SkillName;
					float SkillCooldown = Reward.SkillData->SkillCooldown;
					FName SkillDesc = Reward.SkillData->SkillDescription;

					OnSkillReward.Broadcast(SkillIcon, SkillName, SkillCooldown, SkillDesc);

					Result.Add(Reward);
					--RewardNum;
				}
			}
		}
	}

	Algo::RandomShuffle(StatRewardPool);
	for (int32 i = 0; i < RewardNum; ++i)
	{
		if (StatRewardPool.IsValidIndex(i))
		{
			const FRewardData& Reward = StatRewardPool[i];
			
			if (StatRewardPool[i].RewardType == ERewardType::Stat)
			{
				int32 StatEnumIndex = static_cast<int32>(Reward.StatType);
				float StatValue = Reward.Value;

				OnStatReward.Broadcast(StatEnumIndex, StatValue);
			}

			Result.Add(Reward);
		}
	}

	CurrentReward = Result;

	if (PlayerController)
	{
		PlayerController->EnterRewardMode();

		if (CurrentReward.Num() <= 0)
		{
			PlayerController->ExitRewardMode();
			RemoveReward();
		}
	}
}

void AGC_RewardManager::RemoveReward()
{
	CurrentReward.Empty();
	RewardCursor = -1;

	OnRewardRemove.Broadcast();

	if (PlayerController)
	{
		PlayerController->ExitRewardMode();
	}

	if (PlayerCharacter)
	{
		PlayerCharacter->CheckExp();
	}
}

void AGC_RewardManager::SelectReward()
{
	check(PlayerCharacter);

	if (CurrentReward.IsValidIndex(RewardCursor))
	{
		// UE_LOG(LogTemp, Log, TEXT("ApplyReward : %d"), RewardCursor);
		FRewardData& SelectReward = CurrentReward[RewardCursor];

		switch (SelectReward.RewardType)
		{
		case ERewardType::Skill:
		{
			SelectedRewardID.Add(SelectReward.SkillData->SkillName);

			PlayerCharacter->ChangeSkill(SelectReward.SkillData);
			break;
		}
		case ERewardType::Stat:
		{
			FStatModifier Modifier;
			Modifier.StatType = SelectReward.StatType;
			Modifier.Operation = SelectReward.Op;
			Modifier.Value = SelectReward.Value;

			PlayerCharacter->GetStatComponent()->AddStatModifier(Modifier);
			break;
		}
		case ERewardType::Heal:
		{
			PlayerCharacter->ApplyHeal(SelectReward.Value);
			break;
		}
		}

		RemoveReward();
	}
}

void AGC_RewardManager::MoveRewardCursor(float AxisValue)
{
	if (CurrentReward.Num() > 0)
	{
		const int32 MoveValue = AxisValue;

		OnRewardOutCursor.Broadcast(RewardCursor);

		RewardCursor = FMath::Clamp(RewardCursor + MoveValue, 0, CurrentReward.Num() - 1);

		OnRewardInCursor.Broadcast(RewardCursor);

		// UE_LOG(LogTemp, Warning, TEXT("Cursor : %d"), RewardCursor);
	}
}

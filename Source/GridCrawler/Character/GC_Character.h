// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Game\GC_Grid.h"

#include "GC_Character.generated.h"

class AGC_GridManager;
class AGC_BattleManager;
class UGC_CharacterStatComponent;
class UGC_SkillComponent;
class UGC_SkillData;
class AGC_DamageTextActor;

UENUM()
enum class ETeam : uint8
{
	Neutral,
	Player,
	Enemy,
};

UENUM()
enum class ECharacterState : uint8
{
	Idle,
	Aim,
	Move,
	MoveEnd,
	Skill,
	SkillEnd,
	ForcedMove,
};

UCLASS()
class GRIDCRAWLER_API AGC_Character : public ACharacter
{
	GENERATED_BODY()

public:
	AGC_Character();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void Init(AGC_GridManager* InGridManager, AGC_BattleManager* InBattleManager, const FGridCoord& Coord);

	FORCEINLINE AGC_BattleManager* GetBattleManager() const { return BattleManager; }

	FORCEINLINE FGridCoord GetGridCoord() const { return CurrentCoord; }

	FORCEINLINE void SetGridCoord(const FGridCoord& Coord)
	{
		CurrentCoord.Point.X = Coord.Point.X;
		CurrentCoord.Point.Y = Coord.Point.Y;
	}

	FORCEINLINE ECharacterDirection GetCharacterDirection() const { return CharacterDirection; }

	FORCEINLINE ETeam GetTeam() const { return Team; }

	FORCEINLINE void SetTeam(ETeam NewTeam) { Team = NewTeam; }

	/** Stat */
	UGC_CharacterStatComponent* GetStatComponent() const
	{
		return StatComp;
	}

	float GetStatMaxHp() const;

	float GetStatCurrentHp() const;

	float GetStatAttack() const;

	float GetStatArmor() const;

	float GetStatMoveSpeed() const;

	float GetStatAttackSpeed() const;

	float ApplyDamage(float Damage);

	float ApplyHeal(float HealAmount);

	/** Skill */
	void RequestSkillStep(int32 StepIndex);

	void RequestForcedMove(bool bIsSelf, const TArray<FGridCoord>& Path, float MoveSpeed);

protected:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	FORCEINLINE void SetWantsAim(bool b) { bWantsAim = b; }

	FORCEINLINE ECharacterDirection GetWantsDirection() const { return WantsDirection; }

	FORCEINLINE void SetWantsDirection(ECharacterDirection Dir) { WantsDirection = Dir; }
	
	FORCEINLINE void SetWantsMove(bool b) { bWantsMove = b; }

	FORCEINLINE bool GetWantsSkill() const { return bWantsSkill; }

	FORCEINLINE void SetWantsSkill(bool b) { bWantsSkill = b; }

	FORCEINLINE void SetWantsSkillIndex(int32 Idx) { WantsSkillIndex = Idx; }

	/** Try Movement Function */
	virtual void TryMove(ECharacterDirection InDirection);

	bool CanMoveTo(ECharacterDirection InDirection) const;

	/** Try Attack Function */
	void TrySkill(ECharacterDirection InDirection, const UGC_SkillData* Skill);

	void TrySkillMontage(const UGC_SkillData* Skill);

	virtual void EndSkill(UAnimMontage* AnimMontage, bool bInterrupted);

	/** Try ForcedMove Function */
	virtual void TryForcedMove();

	void EndForcedMove();

	/** Rotate Character to InDirection */
	void UpdateCharacterDirection(ECharacterDirection InDirection);

	virtual void DeathCharacter();

	void RequestRemoveSkillIndicator();

protected:
	/** State Function */
	void SetCharacterState(ECharacterState NewState);

	void DecisionAction();

	ECharacterDirection ResolveActionDirection();

protected:
	/** State */
	UPROPERTY(VisibleAnywhere, Category = "GC|State")
	ECharacterState CharacterState = ECharacterState::Idle;

	UPROPERTY(VisibleAnywhere, Category = "GC|State")
	ECharacterDirection CharacterDirection = ECharacterDirection::Right;

	UPROPERTY(VisibleAnywhere, Category = "GC|State")
	ECharacterDirection WantsDirection = ECharacterDirection::None;
	
	UPROPERTY(VisibleAnywhere, Category = "GC|State")
	bool bWantsAim = false;

	UPROPERTY(VisibleAnywhere, Category = "GC|State")
	bool bWantsMove = false;

	UPROPERTY(VisibleAnywhere, Category = "GC|State")
	bool bWantsSkill = false;

	UPROPERTY(VisibleAnywhere, Category = "GC|State")
	int32 WantsSkillIndex = -1;

	FTimerHandle AimingTimer;

	float AimingTime = 0.12f;

protected:
	/** Manager */
	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_GridManager> GridManager = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "GC|Manager")
	TObjectPtr<AGC_BattleManager> BattleManager = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, Category = "GC|Equip")
	TObjectPtr<UStaticMeshComponent> LeftHandMesh;

	UPROPERTY(VisibleAnywhere, Category = "GC|Equip")
	TObjectPtr<UStaticMeshComponent> RightHandMesh;

protected:
	/** Move */
	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	FGridCoord CurrentCoord;

	// Destination Coord
	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	FGridCoord DestCoord;

	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	FVector MoveStartLocation;

	UPROPERTY(VisibleAnywhere, Category = "GC|Move")
	FVector MoveEndLocation;

	/** ForcedMove */
	UPROPERTY(VisibleAnywhere, Category = "GC|ForcedMove")
	TArray<FGridCoord> ForcedMovePath;

	UPROPERTY(VisibleAnywhere, Category = "GC|ForcedMove")
	int32 ForcedMoveIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "GC|ForcedMove")
	float ForcedMoveSpeed = 0.f;

protected:
	/** Rotate */
	UPROPERTY(VisibleAnywhere, Category = "GC|Rotate")
	bool bIsRotate = false;

	UPROPERTY(VisibleAnywhere, Category = "GC|Rotate")
	FRotator StartRotator;

	UPROPERTY(VisibleAnywhere, Category = "GC|Rotate")
	FRotator EndRotator;

	UPROPERTY(VisibleAnywhere, Category = "GC|Rotate")
	float RotateDuration = 0.15f;

	UPROPERTY(VisibleAnywhere, Category = "GC|Rotate")
	float RotateSpeed = 500.f;

	UPROPERTY(VisibleAnywhere, Category = "GC|Rotate")
	float CurrentRotateTime = 0.f;

protected:
	/** Team*/
	UPROPERTY(VisibleAnywhere, Category = "GC|Team")
	ETeam Team;

protected:
	/** Stat */
	UPROPERTY(VisibleAnywhere, Category = "GC|Stat")
	TObjectPtr<UGC_CharacterStatComponent> StatComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GC|Stat")
	TSubclassOf<AGC_DamageTextActor> DamageTextActorClass = nullptr;

protected:
	/** Skill */
	UPROPERTY(VisibleAnywhere, Category = "GC|Skill")
	TObjectPtr<UGC_SkillComponent> SkillComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Skill")
	TObjectPtr<const UGC_SkillData> CurrentSkill = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "GC|Skill")
	int32 CurrentSkillIndex = -1;

	UPROPERTY(VisibleAnywhere, Category = "GC|Skill")
	int32 SkillInstanceID = -1;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC_Character.h"

#include "Components\CapsuleComponent.h"
#include "Kismet\GameplayStatics.h"
#include "Game\GC_GridManager.h"
#include "Game\GC_BattleManager.h"
#include "Character\GC_CharacterStatComponent.h"
#include "Character\GC_SkillComponent.h"
#include "Skill\GC_SkillData.h"
#include "Character\GC_DamageTextActor.h"

AGC_Character::AGC_Character()
{
	bUseControllerRotationYaw = false;

	// Set Character Mesh Location, Rotation 
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	LeftHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HANDMESH_L"));
	RightHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HANDMESH_R"));
	
	// Set Character Stat Component
	StatComp = CreateDefaultSubobject<UGC_CharacterStatComponent>(TEXT("STAT"));

	// Set Character Skill Component
	SkillComp = CreateDefaultSubobject<UGC_SkillComponent>(TEXT("SKILL"));
}

void AGC_Character::BeginPlay()
{
	Super::BeginPlay();

	// Set Character Direction
	CharacterDirection = GridManager->RotatorToDirection(GetActorRotation());

	// Bind Death Event
	StatComp->OnHpZero.AddUObject(this, &AGC_Character::DeathCharacter);
}

void AGC_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CharacterState == ECharacterState::Move)
	{
		// Constant Movement Interpolation (CurrentLocation -> MoveEndLocation)
		float MoveSpeed = StatComp->GetMoveSpeed();
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, MoveEndLocation, DeltaSeconds, MoveSpeed);
		SetActorLocation(NewLocation);

		// If Arrived At End Location ,End of Movement Interpolation
		if (FVector::Dist(GetActorLocation(), MoveEndLocation) <= MoveSpeed * DeltaSeconds)
		{
			SetActorLocation(MoveEndLocation);
			
			// State Change : MoveEnd
			SetCharacterState(ECharacterState::MoveEnd);
		}
	}

	if (CharacterState == ECharacterState::ForcedMove)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, MoveEndLocation, DeltaSeconds, ForcedMoveSpeed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(GetActorLocation(), MoveEndLocation) <= ForcedMoveSpeed * DeltaSeconds)
		{
			SetActorLocation(MoveEndLocation);
			
			++ForcedMoveIndex;

			TryForcedMove();
		}
	}

	// If Already On The Rotate, Don't Process Rotate
	if (bIsRotate && GetCharacterState() != ECharacterState::Skill)
	{
		// Rotation Interpolation (StartRotator -> EndRotator)
		CurrentRotateTime += DeltaSeconds;
		float Alpha = FMath::Clamp(CurrentRotateTime / RotateDuration, 0.f, 1.f);
		FRotator NewRotator = FMath::Lerp(StartRotator, EndRotator, Alpha);
		SetActorRotation(NewRotator);

		// If Alpha >= 1.f, End of Rotation Interpolation
		if (Alpha >= 1.f)
		{
			SetActorRotation(EndRotator);
			CharacterDirection = GridManager->RotatorToDirection(EndRotator);
			bIsRotate = false;
		}
	}
}

void AGC_Character::Init(AGC_GridManager* InGridManager, AGC_BattleManager* InBattleManager, const FGridCoord& Coord)
{
	// Set Grid Manager
	GridManager = InGridManager;
	BattleManager = InBattleManager;

	// Set Character's Coord And Set Location In Coord
	SetGridCoord(Coord);
	FVector SpawnLocation = GridManager->CoordToLocation(GetGridCoord());
	SetActorLocation(SpawnLocation);

	GridManager->SetOccupyingActor(GetGridCoord(), this);
}

void AGC_Character::UpdateCharacterDirection(ECharacterDirection InDirection)
{
	SetWantsAim(false);

	if (InDirection == ECharacterDirection::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Update Direction is NONE !!"));
		return;
	}

	// Set End Rotator
	EndRotator = GridManager->DirectionToRotator(InDirection);

	// Set Start Rotator
	StartRotator = GetActorRotation();

	// Set Rotate Duration
	FRotator Delta = (EndRotator - StartRotator).GetNormalized();
	float Angle = FMath::Abs(Delta.Yaw);
	RotateDuration = Angle / RotateSpeed;

	// Start Rotation Interpolation
	CharacterDirection = InDirection;
	CurrentRotateTime = 0.f;
	bIsRotate = true;
}

float AGC_Character::GetStatMaxHp() const
{
	return StatComp->GetMaxHp();
}

float AGC_Character::GetStatCurrentHp() const
{
	return StatComp->GetCurrentHp();
}

float AGC_Character::GetStatAttack() const
{
	return StatComp->GetAttack();
}

float AGC_Character::GetStatArmor() const
{
	return StatComp->GetArmor();
}

float AGC_Character::GetStatMoveSpeed() const
{
	return StatComp->GetMoveSpeed();
}

float AGC_Character::GetStatAttackSpeed() const
{
	return StatComp->GetAttackSpeed();
}

float AGC_Character::ApplyDamage(float Damage)
{
	float ActualDamage = StatComp->ApplyDamage(Damage);

	check(DamageTextActorClass);

	FVector Location = GridManager->CoordToLocation(GetGridCoord());
	AGC_DamageTextActor* DmgText
		= GetWorld()->SpawnActor<AGC_DamageTextActor>(DamageTextActorClass, Location, FRotator::ZeroRotator);

	DmgText->SetDamageText(ActualDamage);

	return ActualDamage;
}

float AGC_Character::ApplyHeal(float HealAmount)
{
	return StatComp->ApplyHeal(HealAmount);
}

void AGC_Character::RequestSkillStep(int32 StepIndex)
{
	BattleManager->ProcessSkillEffect(CurrentSkill, SkillInstanceID, StepIndex);
}

void AGC_Character::RequestForcedMove(bool bIsSelf, const TArray<FGridCoord>& Path, float MoveSpeed)
{
	UE_LOG(LogTemp, Warning, TEXT("ForcedMove!"));

	ForcedMovePath = Path;
	ForcedMoveIndex = 0;
	ForcedMoveSpeed = MoveSpeed;
	
	RequestRemoveSkillIndicator();
	TryForcedMove();
}

void AGC_Character::DeathCharacter()
{
	RequestRemoveSkillIndicator();
}

void AGC_Character::RequestRemoveSkillIndicator()
{
	UE_LOG(LogTemp, Warning, TEXT("RequestRemoveSkillIndicator : %d"), SkillInstanceID);

	if (SkillInstanceID != -1)
	{
		for (int32 StepIndex = 0; StepIndex < CurrentSkill->Deliveries.Num(); ++StepIndex)
		{
			BattleManager->RemoveSkillIndicator(SkillInstanceID, StepIndex);
		}
	}
}

void AGC_Character::TryMove(ECharacterDirection InDirection)
{
	SetWantsMove(false);

	// Set Destination Coordinates
	DestCoord = GetGridCoord() + InDirection;

	// Set Destination Tiles's Occupying Actor
	GridManager->SetOccupyingActor(DestCoord, this);
	SetGridCoord(DestCoord);

	// Start Movement Interpolation
	MoveStartLocation = GetActorLocation();
	MoveEndLocation = GridManager->CoordToLocation(DestCoord, GetActorLocation().Z);;

	// State Change : Move
	SetCharacterState(ECharacterState::Move);
}

bool AGC_Character::CanMoveTo(ECharacterDirection InDirection) const
{
	return GridManager && GridManager->IsCanEnterTile(GetGridCoord() + InDirection);
}

void AGC_Character::TrySkill(ECharacterDirection InDirection, const UGC_SkillData* Skill)
{
	check(Skill);

	if (SkillComp->IsSkillInCooldown(WantsSkillIndex, GetWorld()->GetTimeSeconds()))
	{
		SetCharacterState(ECharacterState::SkillEnd);
		return;
	}

	CurrentSkill = Skill;
	CurrentSkillIndex = WantsSkillIndex;

	// Rotate To Attack Direction
	//UpdateCharacterDirection(InDirection);
	SetActorRotation(GridManager->DirectionToRotator(InDirection));
	CharacterDirection = InDirection;

	// State Change : Skill
	// CurrentSkillTime = 0.f;
	SetCharacterState(ECharacterState::Skill);

	SkillComp->SetSkillInUse(CurrentSkillIndex);
	TrySkillMontage(CurrentSkill);
}

void AGC_Character::TrySkillMontage(const UGC_SkillData* Skill)
{
	LeftHandMesh->SetStaticMesh(Skill->SkillWeaponHandL);
	LeftHandMesh->SetRelativeRotation(Skill->SocketRotatorL);
	LeftHandMesh->SetHiddenInGame(false);
	
	RightHandMesh->SetStaticMesh(Skill->SkillWeaponHandR);
	RightHandMesh->SetRelativeRotation(Skill->SocketRotatorR);
	RightHandMesh->SetHiddenInGame(false);

	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	const float AttackSpeed = GetStatAttackSpeed();
	if (AnimInstance)
	{
		if (Skill->SkillMontage)
		{
			AnimInstance->Montage_Play(Skill->SkillMontage, AttackSpeed);

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindUObject(this, &AGC_Character::EndSkill);
			AnimInstance->Montage_SetEndDelegate(OnMontageEnded, Skill->SkillMontage);

			SkillInstanceID =
				BattleManager->IndicateSkillEffect(this, Skill);
			// UE_LOG(LogTemp, Warning, TEXT("SkillInstancedID : %d"), SkillInstanceID);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Skill Montage"));
		}
	}
}

void AGC_Character::EndSkill(UAnimMontage* AnimMontage, bool bInterrupted)
{
	SkillComp->SetSkillExecutedTime(CurrentSkillIndex, GetWorld()->GetTimeSeconds());

	CurrentSkill = nullptr;
	SkillInstanceID = -1;

	SetCharacterState(ECharacterState::SkillEnd);
}

void AGC_Character::TryForcedMove()
{
	if (ForcedMovePath.IsValidIndex(ForcedMoveIndex)
		&& GridManager->IsCanEnterTile(ForcedMovePath[ForcedMoveIndex]))
	{
		// Set Destination Coordinates
		DestCoord = ForcedMovePath[ForcedMoveIndex];

		// Set Destination Tiles's Occupying Actor
		GridManager->SetOccupyingActor(DestCoord, this);
		SetGridCoord(DestCoord);

		// Start Movement Interpolation
		MoveStartLocation = GetActorLocation();
		MoveEndLocation = GridManager->CoordToLocation(DestCoord, GetActorLocation().Z);

		// State Change : Forced Move
		SetCharacterState(ECharacterState::ForcedMove);
	}
	else
	{
		if (CurrentSkillIndex != -1)
		{
			UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
			if (AnimInstance)
			{
				AnimInstance->Montage_Stop(0.1f);
			}
		}
		else
		{
			SetCharacterState(ECharacterState::SkillEnd);
		}
	}
}

void AGC_Character::EndForcedMove()
{

}

void AGC_Character::SetCharacterState(ECharacterState NewState)
{
	CharacterState = NewState;

	if (CharacterState == ECharacterState::Aim)
	{
		// Rotate To Aiming Direction
		UpdateCharacterDirection(WantsDirection);
		
		// Start Aiming End Timer
		GetWorldTimerManager().SetTimer(AimingTimer, this, &AGC_Character::DecisionAction, AimingTime, false);
	}
	else if (CharacterState == ECharacterState::MoveEnd)
	{
		SetWantsMove(false);

		// Move End -> Check Next Action Intention
		DecisionAction();
	}
	else if (CharacterState == ECharacterState::SkillEnd)
	{
		CurrentSkill = nullptr;
		CurrentSkillIndex = -1;
		SetWantsSkill(false);

		// Attack End -> Check Next Action Intention
		DecisionAction();
	}
}

void AGC_Character::DecisionAction()
{
	ECharacterDirection ResolveDirection = ResolveActionDirection();

	if (CharacterState == ECharacterState::Idle)
	{
		// When Want Attack in Idle State, Try Attack To Intended Direction
		if (bWantsSkill)
		{
			TrySkill(ResolveDirection, SkillComp->GetSkillData(WantsSkillIndex));
			return;
		}

		// When Want Move in Idle State, Enter Aim State
		if (bWantsAim)
		{
			SetCharacterState(ECharacterState::Aim);
			return;
		}
	}

	if (CharacterState == ECharacterState::Aim)
	{
		// When Want Attack in Aim State, Try Attack To Intended Direction
		if (bWantsSkill)
		{
			TrySkill(ResolveDirection, SkillComp->GetSkillData(WantsSkillIndex));
			return;
		}

		// When Want Move in Aim State, Try Move To Intended Direction
		if (bWantsMove)
		{
			UpdateCharacterDirection(ResolveDirection);

			if (CanMoveTo(ResolveDirection))
			{
				TryMove(ResolveDirection);
				return;
			}
			else
			{
				TryMove(ECharacterDirection::None);
				return;
			}
		}

		// If No Action Input by End of Aiming End Timer, Enter Idle State
		SetCharacterState(ECharacterState::Idle);
		return;
	}

	if (CharacterState == ECharacterState::MoveEnd)
	{
		// When Want Attack in MoveEnd State, Try Attack To Intended Direction
		if (bWantsSkill)
		{
			TrySkill(ResolveDirection, SkillComp->GetSkillData(WantsSkillIndex));
			return;
		}

		// When Want Move in MoveEnd State, Enter Aim State
		if (bWantsAim)
		{
			SetCharacterState(ECharacterState::Aim);
			return;
		}
		
		// If No Action Input in MoveEnd State, Enter Idle State
		SetCharacterState(ECharacterState::Idle);
		return;
	}

	if (CharacterState == ECharacterState::SkillEnd)
	{
		// When Want Attack in AttackEnd State, Try Attack To Intended Direction
		if (bWantsSkill)
		{
			TrySkill(ResolveDirection, SkillComp->GetSkillData(WantsSkillIndex));
			return;
		}

		// When Want Move in MoveEnd State, Enter Aim State
		if (bWantsAim)
		{
			SetCharacterState(ECharacterState::Aim);
			return;
		}

		// If No Action Input in AttackEnd State, Enter Idle State
		SetCharacterState(ECharacterState::Idle);
		return;
	}
}

ECharacterDirection AGC_Character::ResolveActionDirection()
{
	// Return WantsDirection or CharacterDirection
	if (WantsDirection == ECharacterDirection::None)
		return CharacterDirection;

	return WantsDirection;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GC_Projectile.h"

#include "Game\GC_GridManager.h"
#include "Character\GC_Character.h"
#include "Skill\GC_SkillEffect.h"

// Sets default values
AGC_Projectile::AGC_Projectile()
{
	// Set Components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SetRootComponent(Root);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESHCOMP"));
	StaticMesh->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGC_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGC_Projectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsMove)
	{
		float MoveSpeed = ProjectileParam.Speed;
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, EndLocation, DeltaSeconds, MoveSpeed);
		
		//float Radius = StaticMesh->Bounds.SphereRadius;
		float Radius = 50.f;
		FHitResult HitResult;
		FCollisionQueryParams QueryParam;
		QueryParam.AddIgnoredActor(SkillContext.User);

		bool bHit = GetWorld()->SweepSingleByChannel(OUT HitResult,
			CurrentLocation,
			NewLocation,
			FQuat::Identity,
			ECC_Pawn,
			FCollisionShape::MakeBox(FVector(Radius, Radius, Radius)), QueryParam);

		if (bHit)
		{
			AGC_Character* HitActor = Cast<AGC_Character>(HitResult.GetActor());
			if (HitActor && !HitActors.Contains(HitActor))
			{
				SetActorLocation(HitResult.Location);
				ProcessHitEffect(HitActor);
			}
		}

		SetActorLocation(NewLocation);

		if (FVector::Dist(GetActorLocation(), EndLocation) <= MoveSpeed * DeltaSeconds)
		{
			SetActorLocation(EndLocation);
			bIsMove = false;

			MoveStep();
		}
	}
}

void AGC_Projectile::Init(const FSkillContext& Context, const FProjectileParam& InParam)
{
	// UE_LOG(LogTemp, Log, TEXT("Spawn Projectile"));

	SkillContext = Context;
	GridManager = Context.GridManager;
	User = Context.User;
	ProjectileMoveArea = Context.EffectArea;

	StaticMesh->SetStaticMesh(InParam.ProjectileMesh);
	StaticMesh->IgnoreActorWhenMoving(User, true);

	ProjectileDirection = User->GetCharacterDirection();
	SetActorRotation(GridManager->DirectionToRotator(ProjectileDirection));

	CurrentCoord = User->GetGridCoord();
	SetActorLocation(GridManager->CoordToLocation(CurrentCoord));

	// Init Projectile Param
	ProjectileParam = InParam;

	CurrentRange = 0;

	// Projectile Fire
	MoveStep();
}

void AGC_Projectile::MoveStep()
{
	if (CurrentRange < ProjectileMoveArea.Num() && GridManager->IsInGridBounds(CurrentCoord))
	{
		EndCoord = ProjectileMoveArea[CurrentRange];
		EndLocation = GridManager->CoordToLocation(EndCoord);

		++CurrentRange;
		CurrentCoord = EndCoord;

		// UE_LOG(LogTemp, Log, TEXT("In Move"));
		bIsMove = true;
	}
	else
	{
		ProcessEndEffect();
	}
}

void AGC_Projectile::ProcessHitEffect(AGC_Character* HitActor)
{
	// UE_LOG(LogTemp, Warning, TEXT("Hit Projectile"));

	HitActors.Add(HitActor);

	for (UGC_SkillEffect* Effect : ProjectileParam.HitEffects)
	{
		SkillContext.Target = HitActor;
		Effect->ApplyEffect(SkillContext);
	}

	if (!ProjectileParam.bPiercing)
	{
		Destroy();
	}
}

void AGC_Projectile::ProcessEndEffect()
{
	for (UGC_SkillEffect* Effect : ProjectileParam.EndEffects)
	{
		Effect->ApplyEffect(SkillContext);
	}

	Destroy();
}

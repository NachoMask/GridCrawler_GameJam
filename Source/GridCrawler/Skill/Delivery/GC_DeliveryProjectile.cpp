// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill\Delivery\GC_DeliveryProjectile.h"

#include "Game\GC_GridManager.h"
#include "Character\GC_Character.h"

void UGC_DeliveryProjectile::Execute(const FSkillContext& Context)
{
	//UE_LOG(LogTemp, Log, TEXT("Delivery: Projectile"));

	AGC_GridManager* GridManager = Context.GridManager;
	AGC_Character* User = Context.User;
	UWorld* World = User->GetWorld();

	check(ProjectileClass);

	AGC_Projectile* Projectile = World->SpawnActor<AGC_Projectile>(ProjectileClass);

	ProjectileParam.HitEffects = Effects;
	Projectile->Init(Context, ProjectileParam);
}

void UGC_DeliveryProjectile::GetSkillIndicator(const FSkillContext& Context, TArray<FGridCoord>& Result)
{
	AGC_GridManager* GridManager = Context.GridManager;
	AGC_Character* User = Context.User;

	FGridCoord UserCoord = User->GetGridCoord();

	for (int32 i = 0; i < ProjectileParam.Range; ++i)
	{
		UserCoord += User->GetCharacterDirection();
		Result.Add(UserCoord);
	}
}

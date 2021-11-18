// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyEnemyTW.h"
#include "Neokaiju/Enemies/EnemySubsystem.h"
#include "Engine/World.h"


void UDestroyEnemyTW::Init(UWorld* WorldValue)
{
	UBaseTagWatcher::Init(WorldValue);

	UEnemySubsystem* EnemySubsystem = WorldValue->GetSubsystem<UEnemySubsystem>();
	EnemySubsystem->OnEnemyDestroyed.AddUniqueDynamic(this, &UDestroyEnemyTW::EnemyDestroyed);
}

void UDestroyEnemyTW::GetSubsystemDependencies(TSet<TSubclassOf<USubsystem>>& OutDependencies) const
{
	OutDependencies.Add(UEnemySubsystem::StaticClass());
}


void UDestroyEnemyTW::EnemyDestroyed(AActor* DestroyedEnemy)
{
	if (IsValid(DestroyedEnemy))
	{
		for (FName Tag : DestroyedEnemy->Tags)
		{
			this->TagSeen(Tag);
		}
	}
}

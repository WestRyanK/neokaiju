// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySubsystem.h"
#include "SwarmGrabManager.h"

void UEnemySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	this->SwarmGrabManager = NewObject <USwarmGrabManager>(this);
}

void UEnemySubsystem::EnemyDestroyed(AActor* DestroyedEnemy)
{
	this->OnEnemyDestroyed.Broadcast(DestroyedEnemy);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSubsystem.h"
#include "Neokaiju/Achievements/TagWatchers/BaseTagWatcher.h"
#include "Neokaiju/Achievements/TagWatchers/DestroyStructureTW.h"
#include "Neokaiju/Achievements/TagWatchers/DestroyEnemyTW.h"
#include "Neokaiju/Achievements/TagWatchers/PickupTW.h"
#include "Neokaiju/Core/PhasedGameMode.h"
#include "Engine/World.h"

//const FName UAchievementSubsystem::StartPlayPhaseTimerTag = TEXT("StartPlayPhase");

void UAchievementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	this->TagWatchers.Add(NewObject<UDestroyStructureTW>());
	this->TagWatchers.Add(NewObject<UDestroyEnemyTW>());
	this->TagWatchers.Add(NewObject<UPickupTW>());

	TSet<TSubclassOf<USubsystem>> Dependencies;
	for (UBaseTagWatcher* TagWatcher : this->TagWatchers)
	{
		TagWatcher->GetSubsystemDependencies(Dependencies);
	}

	for (TSubclassOf<USubsystem> Dependency : Dependencies)
	{
		Collection.InitializeDependency(Dependency);
	}

	UWorld* World = this->GetWorld();
	for (UBaseTagWatcher* TagWatcher : this->TagWatchers)
	{
		TagWatcher->Init(World);
		TagWatcher->OnTagSeen.AddUniqueDynamic(this, &UAchievementSubsystem::TagSeen);
	}
}

void UAchievementSubsystem::TagSeen(FName SeenTag)
{
	this->OnTagSeen.Broadcast(SeenTag);
}

void UAchievementSubsystem::AchievementEarned(UBaseAchievement* Achievement)
{
	this->OnAchievementEarned.Broadcast(Achievement);
}

void UAchievementSubsystem::AchievementProgressed(UBaseAchievement* Achievement, int32 CurrentProgressAmount)
{
	this->OnAchievementProgressed.Broadcast(Achievement, CurrentProgressAmount);
}

void UAchievementSubsystem::AchievementTimerStarted(UBaseAchievement* StartedAchievement)
{
	this->OnAchievementTimerStarted.Broadcast(StartedAchievement);
}

void UAchievementSubsystem::AchievementTimerTicked(UBaseAchievement* TickedAchievement, float AchievementTimeElapsed, float AchievementTimeRemaining)
{
	this->OnAchievementTimerTicked.Broadcast(TickedAchievement, AchievementTimeElapsed, AchievementTimeRemaining);
}

void UAchievementSubsystem::AchievementTimerEnded(UBaseAchievement* EndedAchievement)
{
	this->OnAchievementTimerEnded.Broadcast(EndedAchievement);
}

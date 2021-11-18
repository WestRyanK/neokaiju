// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Neokaiju/Achievements/TagWatchers/BaseTagWatcher.h"
#include "Neokaiju/Core/PhasedGameMode.h"
#include "Engine/World.h"
#include "AchievementSubsystem.generated.h"

class UBaseAchievement;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAchievementProgressed, UBaseAchievement*, Achievement, int32, CurrentProgressAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementEarned, UBaseAchievement*, EarnedAchievement);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementTimerStarted, UBaseAchievement*, StartedAchievement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAchievementTimerTicked, UBaseAchievement*, TickedAchievement, float, AchievementTimeElapsed, float, AchievementTimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementTimerEnded, UBaseAchievement*, EndedAchievement);


class UBaseTagWatcher;
/**
 * This is in charge of broadcasting messages of when Achievements are earned or when progress is made towards earning an achievement. It maintains a list of AchievementListeners.
 */
UCLASS()
class NEOKAIJU_API UAchievementSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	UPROPERTY()
		TSet<UBaseTagWatcher*> TagWatchers;

	UPROPERTY(BlueprintAssignable)
		FOnAchievementProgressed OnAchievementProgressed;

	UPROPERTY(BlueprintAssignable)
		FOnAchievementEarned OnAchievementEarned;

	UPROPERTY(BlueprintAssignable)
		FOnAchievementTimerStarted OnAchievementTimerStarted;

	UPROPERTY(BlueprintAssignable)
		FOnAchievementTimerTicked OnAchievementTimerTicked;

	UPROPERTY(BlueprintAssignable)
		FOnAchievementTimerEnded OnAchievementTimerEnded;

	UPROPERTY(BlueprintAssignable)
		FOnTagSeen OnTagSeen;

	UFUNCTION(BlueprintCallable)
		void AchievementEarned(UBaseAchievement* Achievement);

	UFUNCTION(BlueprintCallable)
		void AchievementProgressed(UBaseAchievement* Achievement, int32 ProgressAmount);

	UFUNCTION(BlueprintCallable)
		void AchievementTimerStarted(UBaseAchievement* StartedAchievement);
	UFUNCTION(BlueprintCallable)
		void AchievementTimerTicked(UBaseAchievement* TickedAchievement, float AchievementTimeElapsed, float AchievementTimeRemaining);
	UFUNCTION(BlueprintCallable)
		void AchievementTimerEnded(UBaseAchievement* EndedAchievement);

protected:

	UFUNCTION()
		void TagSeen(FName SeenTag);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Core/BaseCountdownTimer.h"
#include "Engine/Texture2D.h"
#include "Neokaiju/Achievements/AchievementDefinition.h"
#include "BaseAchievement.generated.h"

class APlayLevelGameMode;
class UAchievementSubsystem;

/**
 *
 */
UCLASS(Blueprintable)
class NEOKAIJU_API UBaseAchievement : public UBaseCountdownTimer
{
	GENERATED_BODY()

public:
	UBaseAchievement();

	UPROPERTY(BlueprintReadOnly)
		FAchievementDefinition Definition;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual int32 GetCurrentProgress() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual bool GetIsAchievementEarned() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual bool GetIsAchievementActive() const;

	void LoadProgress(int32 ProgressValue);

protected:
	UPROPERTY(BlueprintReadOnly)
		int32 CurrentProgress;

	UFUNCTION(BlueprintCallable)
		void SetCurrentProgress(int32 CurrentProgressValue, bool bShouldBroadcastEvents = true);

	UFUNCTION(BlueprintCallable)
		void AddToProgress(int32 ProgressToAdd);

	void StartAchievement();

	bool GetWasLevelComplete() const;

	UFUNCTION()
		void PlayPhaseBegan();

	virtual void CountdownStarted();

	virtual void CountdownTick();

	virtual void CountdownCompleted();

	APlayLevelGameMode* GameMode;

	UAchievementSubsystem* AchievementSubsystem;

	bool bHasAlreadyEarnedAchievement = false;
};

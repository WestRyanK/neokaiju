// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Core/BaseCountdownTimer.h"
#include "Engine/DataTable.h"
#include "AchievementCountdownTimer.generated.h"

class UBaseAchievement;

/**
 *
 */
UCLASS()
class NEOKAIJU_API UAchievementCountdownTimer : public UBaseCountdownTimer
{
	GENERATED_BODY()

public:
	UAchievementCountdownTimer();

	virtual void CountdownTick();

	virtual void CountdownCompleted();

	// If any achievements with this UniqueName (RowName) are earned during gameplay,
	// the timer will transition to PostPlayPhase with Success. Otherwise, it will
	// transition with Failure.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSet<FString> SuccessAchievements;

	// If any achievement with this UniqueName (RowName) are earned during gameplay,
	// the corresponding number of seconds are added to the countdown timer.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TMap<FString, float> AddTimeAchievements;

	// If any achievement with this UniqueName (RowName) are earned during gameplay,
	// the countdown timer is set to the corresponding number of seconds.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TMap<FString, float> SetRemainingTimeAchievements;

protected:
	bool bIsSuccessOnCountdownCompleted = false;

	UFUNCTION()
		void OnAchievementEarned(UBaseAchievement* EarnedAchievement);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnCountdownTicked();

};

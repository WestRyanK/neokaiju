// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "BaseCountdownTimer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCountdownTick, UBaseCountdownTimer*, Timer, float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownCompleted, UBaseCountdownTimer*, Timer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownStarted);
/**
 *
 */
UCLASS(Blueprintable)
class NEOKAIJU_API UBaseCountdownTimer : public UObject
{
	GENERATED_BODY()

public:
	UBaseCountdownTimer();

	// How long the countdown will last for before reaching 0.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CountdownDuration;

	// How frequently the remaining time is updated/calculated. This won't affect
	// how quickly the timer actually counts down, just how fast other parts of the
	// application will be notified of the counting down. If it is too big a number,
	// the countdown may end after it actually was supposed to end. You can probably
	// leave this value alone.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CountdownTickRate = 0.1f;

	UFUNCTION(BlueprintCallable)
		void StartCountdown(bool RestartIfRunning = false);

	UFUNCTION(BlueprintCallable)
		void CancelCountdown();

	UFUNCTION(BlueprintCallable)
		void AddTimeToCountdown(float TimeToAdd);

	UFUNCTION(BlueprintCallable)
		void SetRemainingCountdownTime(float TimeToSet);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetTimeElapsed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetTimeRemaining() const;

	UPROPERTY(BlueprintReadOnly)
		bool bIsCountdownRunning;

	UPROPERTY(BlueprintAssignable)
		FOnCountdownTick OnCountdownTick;

	UPROPERTY(BlueprintAssignable)
		FOnCountdownCompleted OnCountdownCompleted;

	UPROPERTY(BlueprintAssignable)
		FOnCountdownStarted OnCountdownStarted;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float NearingEndTime = 10.0f;

protected:
	UFUNCTION()
		virtual void CountdownStarted();

	UFUNCTION()
		virtual void CountdownTick();

	UFUNCTION()
		virtual void CountdownCompleted();

	UFUNCTION()
		virtual void CountdownNearingEnd();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnNearingEnd();

	float CountdownStartTime;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetIsNearEnd() const;

private:
	bool bCountdownNearEndTriggered = false;

	FTimerHandle CountdownTimerHandle;

};

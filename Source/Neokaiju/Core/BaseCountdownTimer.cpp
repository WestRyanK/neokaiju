// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCountdownTimer.h"
#include "Engine/World.h"

UBaseCountdownTimer::UBaseCountdownTimer()
{
}

void UBaseCountdownTimer::StartCountdown(bool RestartIfRunning)
{
	if (RestartIfRunning)
	{
		this->CancelCountdown();
	}

	if (!this->bIsCountdownRunning)
	{
		this->bIsCountdownRunning = true;
		this->CountdownStartTime = this->GetWorld()->GetTimeSeconds();
		this->GetWorld()->GetTimerManager().SetTimer(this->CountdownTimerHandle, this, &UBaseCountdownTimer::CountdownTick, this->CountdownTickRate, true);
		this->CountdownStarted();
	}
}

void UBaseCountdownTimer::CancelCountdown()
{
	if (this->bIsCountdownRunning)
	{
		this->bIsCountdownRunning = false;
		this->GetWorld()->GetTimerManager().ClearTimer(this->CountdownTimerHandle);
	}
}

void UBaseCountdownTimer::CountdownTick()
{
	float TimeRemaining = this->GetTimeRemaining();
	if (TimeRemaining > 0)
	{
		this->OnCountdownTick.Broadcast(this, TimeRemaining);
	}
	else
	{
		this->CountdownCompleted();
	}

	bool bIsWithinNearingEndTime = (TimeRemaining <= this->NearingEndTime);

	if (!bIsWithinNearingEndTime)
	{
		this->bCountdownNearEndTriggered = false;
	}

	if (bIsWithinNearingEndTime && !this->bCountdownNearEndTriggered)
	{
		this->bCountdownNearEndTriggered = true;
		this->CountdownNearingEnd();
	}
}

bool UBaseCountdownTimer::GetIsNearEnd() const
{
	return this->bCountdownNearEndTriggered;
}

float UBaseCountdownTimer::GetTimeElapsed() const
{
	float TimeElapsed = this->GetWorld()->GetTimeSeconds() - this->CountdownStartTime;
	return TimeElapsed;
}

float UBaseCountdownTimer::GetTimeRemaining() const
{
	float TimeElapsed = this->GetTimeElapsed();
	float TimeRemaining = this->CountdownDuration - TimeElapsed;
	return TimeRemaining;
}

void UBaseCountdownTimer::CountdownStarted()
{
	this->OnCountdownStarted.Broadcast();
}

void UBaseCountdownTimer::CountdownCompleted()
{
	this->OnCountdownCompleted.Broadcast(this);
	this->CancelCountdown();
}

void UBaseCountdownTimer::AddTimeToCountdown(float TimeToAdd)
{
	if (this->bIsCountdownRunning)
	{
		this->CountdownDuration += TimeToAdd;
	}
}

void UBaseCountdownTimer::SetRemainingCountdownTime(float TimeToSet)
{
	if (this->bIsCountdownRunning)
	{
		this->CountdownDuration = this->GetTimeElapsed() + TimeToSet;
	}
}

void UBaseCountdownTimer::CountdownNearingEnd()
{
	OnNearingEnd();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAchievement.h"
#include "Engine/World.h"
#include "Neokaiju/Achievements/AchievementSubsystem.h"
#include "Neokaiju/Core/PlayLevelGameMode.h"
#include "Engine/GameEngine.h"

UBaseAchievement::UBaseAchievement()
{
	if (IsValid(this->GetWorld()))
	{
		this->GameMode = Cast<APlayLevelGameMode>(this->GetWorld()->GetAuthGameMode());
		if (IsValid(this->GameMode))
		{
			this->GameMode->OnPlayPhaseBegan.AddUniqueDynamic(this, &UBaseAchievement::PlayPhaseBegan);
		}

		this->AchievementSubsystem = this->GetWorld()->GetSubsystem<UAchievementSubsystem>();
	}
}

void UBaseAchievement::PlayPhaseBegan()
{
	if (this->Definition.bDoesTimerStartOnBeginPlayPhase)
	{
		this->StartAchievement();
	}
}

bool UBaseAchievement::GetIsAchievementEarned() const
{
	if (!this->Definition.bIsEnabled)
	{
		return false;
	}

	bool bWasProgressRequirementMet;
	switch (this->Definition.ProgressRequirementToEarn)
	{
	case ECompare::GreaterEquals:
		bWasProgressRequirementMet = (this->CurrentProgress >= this->Definition.RequiredProgressToEarn);
		break;
	case ECompare::LessEquals:
		bWasProgressRequirementMet = (this->CurrentProgress <= this->Definition.RequiredProgressToEarn);
		break;
	default:
		bWasProgressRequirementMet = false;
		break;
	}

	bool bWasLevelCompletionConditionStatisfied = (!this->Definition.bIsLevelCompletionRequiredToEarn || this->GetWasLevelComplete());

	return (bWasProgressRequirementMet && bWasLevelCompletionConditionStatisfied);
}

bool UBaseAchievement::GetWasLevelComplete() const
{
	bool bWasLevelComplete = false;
	if (IsValid(GameMode))
	{
		bWasLevelComplete = (GameMode->PlayResult == EGameModePlayResult::Success);
	}

	return bWasLevelComplete;
}

int32 UBaseAchievement::GetCurrentProgress() const
{
	return this->CurrentProgress;
}

bool UBaseAchievement::GetIsAchievementActive() const
{
	if (!this->Definition.bIsEnabled)
	{
		return false;
	}

	bool bCanProgressIncrease = false;
	if (IsValid(this->GameMode))
	{
		bCanProgressIncrease = this->GameMode->CanScoreBeChanged();
	}

	return (!this->Definition.bIsAchievementTimed || this->bIsCountdownRunning) && bCanProgressIncrease;
}

void UBaseAchievement::SetCurrentProgress(int32 CurrentProgressValue, bool bShouldBroadcastEvents)
{
	if (this->GetIsAchievementActive())
	{
		this->CurrentProgress = CurrentProgressValue;

		if (bShouldBroadcastEvents && !this->bHasAlreadyEarnedAchievement)
		{
			this->AchievementSubsystem->AchievementProgressed(this, this->CurrentProgress);
			if (this->GetIsAchievementEarned())
			{
				this->AchievementSubsystem->AchievementEarned(this);
				this->bHasAlreadyEarnedAchievement = true;
			}
		}
	}

}

void UBaseAchievement::AddToProgress(int32 ProgressToAdd)
{
	this->SetCurrentProgress(ProgressToAdd + this->CurrentProgress, true);
}

void UBaseAchievement::StartAchievement()
{
	if (this->Definition.bIsAchievementTimed)
	{
		this->CountdownDuration = this->Definition.TimeLimitSeconds;
		this->StartCountdown();
	}
}

void UBaseAchievement::CountdownStarted()
{
	UBaseCountdownTimer::CountdownStarted();
	this->AchievementSubsystem->AchievementTimerStarted(this);
}

void UBaseAchievement::CountdownTick()
{
	UBaseCountdownTimer::CountdownTick();
	this->AchievementSubsystem->AchievementTimerTicked(this, this->GetTimeElapsed(), this->GetTimeRemaining());
}

void UBaseAchievement::CountdownCompleted()
{
	UBaseCountdownTimer::CountdownCompleted();
	
	this->AchievementSubsystem->AchievementTimerEnded(this);
}

void UBaseAchievement::LoadProgress(int32 ProgressValue)
{
	this->CurrentProgress = ProgressValue;
	if (this->GetIsAchievementEarned())
	{
		this->bHasAlreadyEarnedAchievement = true;
	}
}

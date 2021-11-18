// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementCountdownTimer.h"
#include "Engine/World.h"
#include "Neokaiju/Achievements/AchievementSubsystem.h"
#include "Neokaiju/Achievements/BaseAchievement.h"
#include "Neokaiju/Core/PhasedGameMode.h"

UAchievementCountdownTimer::UAchievementCountdownTimer()
{
	if (IsValid(this->GetWorld()))
	{
		UAchievementSubsystem* Subsystem = this->GetWorld()->GetSubsystem<UAchievementSubsystem>();
		Subsystem->OnAchievementEarned.AddUniqueDynamic(this, &UAchievementCountdownTimer::OnAchievementEarned);
	}
}

void UAchievementCountdownTimer::OnAchievementEarned(UBaseAchievement* EarnedAchievement)
{
	UE_LOG(LogTemp, Display, TEXT("Earned achievement noticed: %s"), *EarnedAchievement->Definition.DisplayName);

	if (this->SuccessAchievements.Contains(EarnedAchievement->Definition.UniqueName))
	{
		this->bIsSuccessOnCountdownCompleted = true;
	}

	float* AdditionalTime = this->AddTimeAchievements.Find(EarnedAchievement->Definition.UniqueName);
	if (AdditionalTime)
	{
		this->AddTimeToCountdown(*AdditionalTime);
	}
	
	float* SetTime = this->SetRemainingTimeAchievements.Find(EarnedAchievement->Definition.UniqueName);
	if (SetTime)
	{
		this->SetRemainingCountdownTime(*SetTime);
	}
}

void UAchievementCountdownTimer::CountdownTick()
{
	Super::CountdownTick();

	this->OnCountdownTicked();
}

void UAchievementCountdownTimer::CountdownCompleted()
{
	Super::CountdownCompleted();

	APhasedGameMode* GameMode = Cast<APhasedGameMode>(this->GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		if (this->bIsSuccessOnCountdownCompleted)
		{
			GameMode->BeginPostPlayPhase(EGameModePlayResult::Timeout);
		}
		else
		{
			GameMode->BeginPostPlayPhase(EGameModePlayResult::Failure);
		}
	}
}

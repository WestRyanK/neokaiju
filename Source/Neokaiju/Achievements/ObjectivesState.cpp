// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectivesState.h"
#include "Neokaiju/Achievements/AchievementSubsystem.h"
#include "Neokaiju/Achievements/BaseAchievement.h"
#include "Engine/World.h"

UObjectivesState::UObjectivesState()
{
	if (IsValid(this->GetWorld()))
	{
		UAchievementSubsystem* Subsystem = this->GetWorld()->GetSubsystem<UAchievementSubsystem>();
		Subsystem->OnAchievementEarned.AddUniqueDynamic(this, &UObjectivesState::OnAchievementEarned);
	}
}

TArray<UBaseAchievement*> UObjectivesState::GetCurrentObjectives() const
{
	TArray<UBaseAchievement*> CurrentObjectives;
	TArray<UBaseAchievement*> EarnedAchievements;
	this->GetEarnedAchievements(EarnedAchievements);

	for (UBaseAchievement* Achievement : this->AchievementsList)
	{
		if (this->PermanentObjectives.Contains(Achievement->Definition.UniqueName))
		{
			CurrentObjectives.Add(Achievement);
		}
	}

	for (UBaseAchievement* Achievement : this->AchievementsList)
	{
		if (!this->PermanentObjectives.Contains(Achievement->Definition.UniqueName))
		{
			if (!EarnedAchievements.Contains(Achievement))
			{
				CurrentObjectives.Add(Achievement);
				break;
			}
			else if (this->bKeepObjectivesWhenCompleted)
			{
				CurrentObjectives.Add(Achievement);
			}
		}
	}

	return CurrentObjectives;
}

void UObjectivesState::OnAchievementEarned(UBaseAchievement* EarnedAchievement)
{
	if (this->AchievementsList.Contains(EarnedAchievement))
	{
		this->OnObjectiveChanged.Broadcast();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HighScoreSave.h"
#include "Neokaiju/KaijuCharacter/KaijuScoreState.h"
#include "Neokaiju/Achievements/AchievementsState.h"
#include "Neokaiju/Achievements/BaseAchievement.h"


void UHighScoreSave::Update(AKaijuScoreState* KaijuScoreState)
{
	if (!IsValid(KaijuScoreState))
	{
		return;
	}

	if (KaijuScoreState->GetLongestStreak() > this->LongestStreak)
	{
		this->LongestStreak = KaijuScoreState->LongestStreak;
	}

	if (KaijuScoreState->GetTotalScore() > this->HighScore)
	{
		this->HighScore = KaijuScoreState->GetTotalScore();
	}

	TArray<FString> AchievementNames;
	KaijuScoreState->AchievementsState->EarnableAchievements.GetKeys(AchievementNames);
	for (FString AchievementName : AchievementNames)
	{
		int32 AchievementProgress = KaijuScoreState->AchievementsState->EarnableAchievements[AchievementName]->GetCurrentProgress();

		if (!this->MaxAchievementProgresses.Contains(AchievementName))
		{
			this->MaxAchievementProgresses.Add(AchievementName, AchievementProgress);
		}
		else
		{
			if (AchievementProgress > this->MaxAchievementProgresses[AchievementName])
			{
				this->MaxAchievementProgresses.Add(AchievementName, AchievementProgress);
			}
		}
	}
}

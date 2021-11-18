// Fill out your copyright notice in the Description page of Project Settings.


#include "StreakAchievement.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Neokaiju/KaijuCharacter/KaijuScoreState.h"

const int MS_IN_SEC = 1000;

UStreakAchievement::UStreakAchievement()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(Controller))
	{
		AKaijuScoreState* ScoreState = Controller->GetPlayerState<AKaijuScoreState>();
		if (IsValid(ScoreState))
		{
			ScoreState->OnCurrentStreakChanged.AddUniqueDynamic(this, &UStreakAchievement::CurrentStreakChanged);
		}
	}
}

void UStreakAchievement::CurrentStreakChanged(float CurrentStreakValue)
{
	int32 ProgressMs = CurrentStreakValue * MS_IN_SEC;
	if (!this->GetIsAchievementEarned() && CurrentStreakValue > 0)
	{
		this->SetCurrentProgress(ProgressMs, true);
	}
}

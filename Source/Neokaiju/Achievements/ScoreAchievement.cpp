// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreAchievement.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Neokaiju/KaijuCharacter/KaijuScoreState.h"


UScoreAchievement::UScoreAchievement()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(Controller))
	{
		AKaijuScoreState* ScoreState = Controller->GetPlayerState<AKaijuScoreState>();
		if (IsValid(ScoreState))
		{
			ScoreState->OnTotalScoreChanged.AddUniqueDynamic(this, &UScoreAchievement::TotalScoreChanged);
		}
	}
}

void UScoreAchievement::TotalScoreChanged(float NewScore)
{
	// We have to divide by 1000 because we're overflowing at $2 billion dollars.
	float ScoreInThousands = NewScore / UScoreAchievement::THOUSAND_DOLLARS;
	this->SetCurrentProgress(ScoreInThousands, true);
}

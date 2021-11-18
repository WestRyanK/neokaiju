// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Achievements/BaseAchievement.h"
#include "ScoreAchievement.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UScoreAchievement : public UBaseAchievement
{
	GENERATED_BODY()

public:
	UScoreAchievement();

	const float THOUSAND_DOLLARS = 1000.0f;

protected:
	UFUNCTION()
		void TotalScoreChanged(float NewScore);
	
};

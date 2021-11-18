// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighScoreSave.generated.h"

class AKaijuScoreState;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API UHighScoreSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	// The longest continuous streak a player has achieved.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float LongestStreak;

	// The highest score a player has achieved.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float HighScore;

	// Maps Achievement UniqueNames to the highest amount of progress the player has made towards that achievement.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FString, int32> MaxAchievementProgresses;

	UFUNCTION()
		void Update(AKaijuScoreState* KaijuScoreState);
};

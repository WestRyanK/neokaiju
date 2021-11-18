// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Achievements/BaseAchievement.h"
#include "StreakAchievement.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UStreakAchievement : public UBaseAchievement
{
	GENERATED_BODY()
	
public:
	UStreakAchievement();

protected:
	UFUNCTION()
		void CurrentStreakChanged(float CurrentStreakValue);
};

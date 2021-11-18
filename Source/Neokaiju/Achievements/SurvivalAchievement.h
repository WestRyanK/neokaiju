// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Achievements/BaseAchievement.h"
#include "SurvivalAchievement.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API USurvivalAchievement : public UBaseAchievement
{
	GENERATED_BODY()
	
public:
	USurvivalAchievement();

protected:

	UFUNCTION()
		void TagSeen(FName SeenTag);
};
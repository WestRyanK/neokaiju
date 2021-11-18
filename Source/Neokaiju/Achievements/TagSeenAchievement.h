// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Achievements/BaseAchievement.h"
#include "TagSeenAchievement.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API UTagSeenAchievement : public UBaseAchievement
{
	GENERATED_BODY()

public:
	UTagSeenAchievement();

protected:
	UFUNCTION()
		void TagSeen(FName SeenTag);
};

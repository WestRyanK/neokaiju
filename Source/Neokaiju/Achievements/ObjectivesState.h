// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Achievements/AchievementsState.h"
#include "ObjectivesState.generated.h"

class UBaseAchievement;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveChanged);
/**
 *
 */
UCLASS()
class NEOKAIJU_API UObjectivesState : public UAchievementsState
{
	GENERATED_BODY()

public:
	UObjectivesState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual TArray<UBaseAchievement*> GetCurrentObjectives() const;

	UPROPERTY(BlueprintAssignable)
		FOnObjectiveChanged OnObjectiveChanged;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bKeepObjectivesWhenCompleted;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FString> PermanentObjectives;

protected:
	UFUNCTION()
		void OnAchievementEarned(UBaseAchievement* EarnedAchievement);

};

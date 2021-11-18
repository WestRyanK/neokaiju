// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AchievementsState.generated.h"

class UBaseAchievement;
class UDataTable;
/**
 *
 */
UCLASS(Blueprintable)
class NEOKAIJU_API UAchievementsState : public UObject
{
	GENERATED_BODY()

public:
	UAchievementsState();

	UPROPERTY(BlueprintReadOnly)
		TMap<FString, UBaseAchievement*> EarnableAchievements;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UDataTable* EarnableAchievementsTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 HighlightedAchievementCount;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		void GetEarnedAchievements(TArray<UBaseAchievement*>& EarnedAchievements) const;

	virtual void PostInitProperties();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TArray<UBaseAchievement*> GetHighlightedAchievements() const;

	UFUNCTION(BlueprintCallable)
		void LoadProgresses(TMap<FString, int32> AchievementProgresses);

protected:
	void SelectHighlightedAchievements();

	void InitAchievements();

	TArray<UBaseAchievement*> AchievementsList;

	TArray<UBaseAchievement*> HighlightedAchievements;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "AchievementDefinition.generated.h"

class UBaseAchievement;

UENUM(BlueprintType)
enum class ECompare : uint8
{
	None				UMETA(DisplayName = "None"),
	LessEquals			UMETA(DisplayName = "Less Than or Equals"),
	GreaterEquals		UMETA(DisplayName = "Greater Than or Equals"),
};
/**
 * This contains key information about different achievements, such as their name, description, icon, and how much progress needs to be made to earn it.
 * Represents an achievement that can be earned in game.
 * If the player earns multiple achievements, the priority of which achievement
 * gets displayed is determined by the order of the Data Table rows that contain
 * the achievements. The earned achievement that shows up first within this table
 * is the one that takes priority.
 */
USTRUCT()
struct NEOKAIJU_API FAchievementDefinition : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UBaseAchievement> AchievementClass;
	
	// The name that this Achievement is called in game.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FString DisplayName;

	// A short description of how to earn the Achievement, or a catchy phrase to go with it.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FString Description;

	//  The TagName is used to link achievements to actors in game.
	// To link an actor to an achievement, add this tag to the Actor's Actor Tags (not GameplayTags).
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName TagName;

	// The number of times something must be done in order to earn this achievement. What, exactly, that
	// needs to be done is determined by AchievementProgressListeners.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 RequiredProgressToEarn;

	// Whether the progress amount must be greater than or less than the RequiredCountToEarn in order to
	// earn the achievement. Usually achievements that require less than a certain amount of progress
	// are also used in conjunction with bIsLevelCompletRequiredToEarn. For example, completing the
	// level while having destroyed 0 swarms.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		ECompare ProgressRequirementToEarn = ECompare::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bIsLevelCompletionRequiredToEarn = false;

	// The amount of points to award the player when they complete the achievement.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float CompletionRewardValue;

	// If true, the achievement must be earned within a certain amount of time specified by TimeLimitSeconds.
	// The timer starts when the AchievementTimerStartTag is seen. This tag may belong to certain objects,
	// or could be triggered by things such as the start of the game.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bIsAchievementTimed = false;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bIsAchievementTimed"))
		bool bLessThanTimeLimit = true;

	// The achievement must be completed within this many seconds from when the achievement timer starts.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bIsAchievementTimed"))
		float TimeLimitSeconds = 0.0f;

	// The timer for completing the timed achievement starts when this tag is seen.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bIsAchievementTimed"))
		FName AchievementTimerStartTag;

	// If true, the timer starts counting down when the PlayPhase begins. Otherwise, the timer
	// starts.... not sure...
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bIsAchievementTimed"))
		bool bDoesTimerStartOnBeginPlayPhase;

	// A template format string for modifying how progress for this achievement is displayed.
	// {CurrentProgress} will be replaced with the scaled value of current progress (scaled by ProgressDisplayMultiplier)
	// {RequiredProgress} will be replaced with the scaled value of the required progress to complete the achievement.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FText ProgressDisplayFormat = FText::FromString(TEXT("{CurrentProgress}/{RequiredProgress}"));

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ProgressDisplayMultiplier = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 ProgressDisplayDecimalDigits = 0;

	// A reference to the End Card Texture that is used when you finish the level and earn this achievement.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* CardImage;

	// A reference to the Icon Texture that is used when notifying the player that the achievement was just earned.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* Icon;

	// Whether the achievement is used in game at all. If disabled, it never even gets included in the game.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bIsEnabled = true;

	// Will the player be notified of progress towards this achievement?
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bIsProgressVisible = true;

	// Will the player be notified when they earn this achievement?
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bIsEarnedVisible = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bPreFinalForm = false;

	UPROPERTY()
		FString UniqueName;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Neokaiju/Core/SaveGame/SaveableInterface.h"
#include "KaijuScoreState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreStateChanged, float, NewScoreStateValue);

class UAchievementsState;
class UObjectivesState;
/**
 *
 */
UCLASS()
class NEOKAIJU_API AKaijuScoreState : public APlayerState, public ISaveableInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly)
		UAchievementsState* AchievementsState;

	UPROPERTY(BlueprintReadOnly)
		UObjectivesState* ObjectivesState;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetTotalScore() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetTotalScoreWithoutAchievementPoints() const;

	UPROPERTY(BlueprintGetter = GetMaxBonusCooldownTimes)
		TArray<float> maxBonusCooldownTimes = {
			100.0f, //1x, won't count down
			2.0f, //2x
			1.5f, //3x
			1.0f //4x
	};

	UFUNCTION(BlueprintGetter)
		TArray<float> GetMaxBonusCooldownTimes() const;
	UFUNCTION(BlueprintCallable)
		float GetMaxBonusCooldownTime() const;
	UPROPERTY(BlueprintGetter = GetPropertyDamageDealt, BlueprintSetter = SetPropertyDamageDealt)
		float PropertyDamageDealt = 0.0f;
	UFUNCTION(BlueprintGetter)
		float GetPropertyDamageDealt() const;
	UFUNCTION(BlueprintSetter)
		void SetPropertyDamageDealt(float PropertyDamageDealtValue);

	UPROPERTY(BlueprintGetter = GetBabyKaijusSaved, BlueprintSetter = SetBabyKaijusSaved)
		float BabyKaijusSaved = 0.0f;
	UFUNCTION(BlueprintGetter)
		float GetBabyKaijusSaved() const;
	UFUNCTION(BlueprintSetter)
		void SetBabyKaijusSaved(float BabyKaijusSavedValue);

	UPROPERTY(BlueprintGetter = GetTimeBonusMultiplier, BlueprintSetter = SetTimeBonusMultiplier)
		float TimeBonusMultiplier = 1.0f;
	UFUNCTION(BlueprintGetter)
		float GetTimeBonusMultiplier();
	UFUNCTION(BlueprintSetter)
		void SetTimeBonusMultiplier(float BonusMultiplierValue);

	UPROPERTY(BlueprintGetter = GetBonusRemainingTime, BlueprintSetter = SetBonusRemainingTime)
		float BonusRemainingTime = 0.0f;
	UFUNCTION(BlueprintGetter)
		float GetBonusRemainingTime();
	UFUNCTION(BlueprintSetter)
		void SetBonusRemainingTime(float BonusTimeRemainingValue);

	UPROPERTY(BlueprintGetter = GetBlocksDestroyedForBonus, BlueprintSetter = SetBlocksDestroyedForBonus)
		float BlocksDestroyedForBonus = 0.0f;
	UFUNCTION(BlueprintGetter)
		float GetBlocksDestroyedForBonus();
	UFUNCTION(BlueprintSetter)
		void SetBlocksDestroyedForBonus(float BlocksDestroyedForBonusValue);

	UPROPERTY(BlueprintGetter = GetLongestStreak, BlueprintSetter = SetLongestStreak)
		float LongestStreak = 0.0f;
	UFUNCTION(BlueprintGetter)
		float GetLongestStreak();
	UFUNCTION(BlueprintSetter)
		void SetLongestStreak(float LongestStreakValue);

	UPROPERTY(BlueprintGetter = GetCurrentStreak, BlueprintSetter = SetCurrentStreak)
		float CurrentStreak = 0.0f;
	UFUNCTION(BlueprintGetter)
		float GetCurrentStreak();
	UFUNCTION(BlueprintSetter)
		void SetCurrentStreak(float CurrentStreakValue);

	UPROPERTY(BlueprintAssignable)
		FOnScoreStateChanged OnTotalScoreChanged;
	UPROPERTY(BlueprintAssignable)
		FOnScoreStateChanged OnPropertyDamageDealtChanged;
	UPROPERTY(BlueprintAssignable)
		FOnScoreStateChanged OnBabyKaijusSavedChanged;
	UPROPERTY(BlueprintAssignable)
		FOnScoreStateChanged OnTimeBonusMultiplierChanged;
	UPROPERTY(BlueprintAssignable)
		FOnScoreStateChanged OnBonusRemainingTimeChanged;
	UPROPERTY(BlueprintAssignable)
		FOnScoreStateChanged OnBlocksDestroyedForBonusChanged;
	UPROPERTY(BlueprintAssignable)
		FOnScoreStateChanged OnHighestBonusChanged;
	UPROPERTY(BlueprintAssignable)
		FOnScoreStateChanged OnLongestStreakChanged;
	UPROPERTY(BlueprintAssignable)
		FOnScoreStateChanged OnCurrentStreakChanged;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UAchievementsState> AchievementsStateClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UObjectivesState> ObjectivesStateClass;
	UPROPERTY()
		float maxBlocksDestroyedForBonus = 20.0f;
	UPROPERTY()
		float maxTimeBonusMultiplier = 4.0f;
	UPROPERTY()
		float minTimeBonusMultiplier = 1.0f;
	UPROPERTY()
		float cooldownRefreshRate = .01f;

	UFUNCTION()
		void OnStructureDestroyed(ABreakableStructure* DestroyedStructure);

	UFUNCTION()
		void TotalScoreChanged();

	UFUNCTION()
		void BonusCooldown();

	UFUNCTION()
		void BonusStreakRefresh();

	FTimerHandle TimerHandle_RefreshCooldown;

	//All time rates are in seconds, not milliseconds

	UFUNCTION()
		void AddSavedBabyKaiju();

	UFUNCTION()
		void UpdateHighScore();
};

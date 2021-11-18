// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "TimerManager.h"
#include "LevelChangeAbility.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API ULevelChangeAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	ULevelChangeAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TMap<int32, TSubclassOf<UGameplayEffect>> SpecialLevelEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> LevelChangeEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> PostLevelChangeEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SlowdownMagnitude;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SlomoDuration;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnPreLevelChange(float OldLevel, float NewLevel);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnPostLevelChange(float OldLevel, float NewLevel);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
	FTimerHandle TimerHandle;

	float OldCharacterLevel;

	float NewCharacterLevel;

	EMovementMode OriginalMovementMode;

	UPROPERTY()
		FActiveGameplayEffectHandle CurrentSpecialLevelEffectHandle;

	void UpdateSpecialLevelEffect();

	UFUNCTION()
		void ApplyLevel();

	UFUNCTION()
		void OnPost();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Tasks/BaseAbilityTask.h"
#include "RepeatInfiniteAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRepeatInfiniteActionDelegate, int32, ActionNumber);
/**
 * 
 */
UCLASS()
class NEOKAIJU_API URepeatInfiniteAbilityTask : public UBaseAbilityTask
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FRepeatInfiniteActionDelegate		OnPerformAction;

	void PerformAction();

	/** Start a task that repeats an action or set of actions. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URepeatInfiniteAbilityTask* RepeatAction(UGameplayAbility* OwningAbility, float TimeBetweenActions);

	virtual void Activate() override;

protected:
	int32 ActionPerformancesDesired;
	int32 ActionCounter;
	float TimeBetweenActions;

	/** Handle for efficient management of PerformAction timer */
	FTimerHandle TimerHandle_PerformAction;

	virtual void OnDestroy(bool AbilityIsEnding) override;
};

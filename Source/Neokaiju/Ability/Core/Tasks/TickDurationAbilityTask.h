// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Tasks/BaseAbilityTask.h"
#include "TickDurationAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTickDurationAbilityTaskDelegate, float, DeltaTime);
/**
 *
 */
UCLASS()
class NEOKAIJU_API UTickDurationAbilityTask : public UBaseAbilityTask
{
	GENERATED_BODY()

public:
	UTickDurationAbilityTask();

	UPROPERTY(BlueprintAssignable)
		FTickDurationAbilityTaskDelegate OnDurationTick;

	UPROPERTY(BlueprintAssignable)
		FTickDurationAbilityTaskDelegate OnDurationFinished;

	UPROPERTY(BlueprintReadWrite)
		float TickDuration;

	/** Start a task that repeats an action or set of actions. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UTickDurationAbilityTask* TickForDuration(UGameplayAbility* OwningAbility, float TickDurationValue = -1);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

protected:
	float StartTimeSeconds;

	float EndTimeSeconds;

	bool bDoesTickForever;
};

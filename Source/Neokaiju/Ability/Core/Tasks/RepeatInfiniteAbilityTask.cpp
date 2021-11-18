// Fill out your copyright notice in the Description page of Project Settings.


#include "RepeatInfiniteAbilityTask.h"

#include "TimerManager.h"

void URepeatInfiniteAbilityTask::PerformAction()
{
	OnPerformAction.Broadcast(ActionCounter);
	ActionCounter++;
}

URepeatInfiniteAbilityTask* URepeatInfiniteAbilityTask::RepeatAction(UGameplayAbility* OwningAbility, float InTimeBetweenActions)
{
	URepeatInfiniteAbilityTask* Task = NewAbilityTask<URepeatInfiniteAbilityTask>(OwningAbility);

	Task->TimeBetweenActions = InTimeBetweenActions;
	Task->ActionCounter = 0;

	return Task;
}

void URepeatInfiniteAbilityTask::Activate()
{
	PerformAction();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_PerformAction, this, &URepeatInfiniteAbilityTask::PerformAction, TimeBetweenActions, true);
}

void URepeatInfiniteAbilityTask::OnDestroy(bool AbilityIsEnding)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_PerformAction);

	Super::OnDestroy(AbilityIsEnding);
}

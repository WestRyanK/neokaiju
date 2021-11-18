// Fill out your copyright notice in the Description page of Project Settings.


#include "TickDurationAbilityTask.h"



UTickDurationAbilityTask::UTickDurationAbilityTask()
{
	this->bTickingTask = true;
}

UTickDurationAbilityTask* UTickDurationAbilityTask::TickForDuration(UGameplayAbility* OwningAbility, float TickDurationValue)
{
	UTickDurationAbilityTask* Task = NewAbilityTask<UTickDurationAbilityTask>(OwningAbility);
	Task->TickDuration = TickDurationValue;
	Task->bDoesTickForever = (TickDurationValue < 0);

	return Task;
}

void UTickDurationAbilityTask::Activate()
{
	this->StartTimeSeconds = this->GetWorld()->GetTimeSeconds();
	this->EndTimeSeconds = this->StartTimeSeconds + this->TickDuration;
}

void UTickDurationAbilityTask::TickTask(float DeltaTime)
{
	if (this->bDoesTickForever || this->GetWorld()->GetTimeSeconds() < this->EndTimeSeconds)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			this->OnDurationTick.Broadcast(DeltaTime);
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			this->OnDurationFinished.Broadcast(DeltaTime);
		}
		this->EndTask();
	}
}

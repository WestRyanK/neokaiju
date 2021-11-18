// Fill out your copyright notice in the Description page of Project Settings.


#include "InputReleaseOrDelayAbilityTask.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"

UInputReleaseOrDelayAbilityTask::UInputReleaseOrDelayAbilityTask()
{
	this->bTickingTask = true;
}

UInputReleaseOrDelayAbilityTask* UInputReleaseOrDelayAbilityTask::WaitInputReleaseOrDelay(
	UGameplayAbility* OwningAbility,
	float DelayTimeSeconds)
{
	UInputReleaseOrDelayAbilityTask* Task = NewAbilityTask<UInputReleaseOrDelayAbilityTask>(OwningAbility);
	Task->DelayTimeSeconds = DelayTimeSeconds;
	Task->bDoesEndOnRelease = true;

	return Task;
}

void UInputReleaseOrDelayAbilityTask::Activate()
{
	Super::Activate();
	this->StartTimeSeconds = this->GetWorld()->GetTimeSeconds();
}

void UInputReleaseOrDelayAbilityTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (!this->IsFinished())
	{
		float TimeRemaining = this->GetTimeRemaining();
		if (TimeRemaining > 0)
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				this->OnTick.Broadcast(TimeRemaining);
			}
		}
		else
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				this->OnComplete.Broadcast(TimeRemaining);
			}

			this->EndTask();
		}
	}
}

void UInputReleaseOrDelayAbilityTask::OnReleaseNoticed()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		this->OnCancel.Broadcast(this->GetTimeRemaining());
	}
	Super::OnReleaseNoticed();
}

float UInputReleaseOrDelayAbilityTask::GetTimeRemaining() const
{
	float TimeRemaining = (this->StartTimeSeconds + this->DelayTimeSeconds) - this->GetWorld()->GetTimeSeconds();
	return TimeRemaining;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "InputPressedAbilityTask.h"

UInputPressedAbilityTask* UInputPressedAbilityTask::InputPressed(UGameplayAbility* OwningAbility, EAbilityInputID Input)
{
	UInputPressedAbilityTask* Task = NewAbilityTask<UInputPressedAbilityTask>(OwningAbility);
	Task->WaitForInputID = Input;

	return Task;
}

void UInputPressedAbilityTask::OnPressNoticed()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		this->OnPressed.Broadcast();
	}

	Super::OnPressNoticed();
}

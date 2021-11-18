// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitForSpecificInputAbilityTask.h"
#include "Neokaiju/Ability/Core/Tasks/TickDurationAbilityTask.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"

UWaitForSpecificInputAbilityTask::UWaitForSpecificInputAbilityTask()
{
	this->bTickingTask = true;
}

void UWaitForSpecificInputAbilityTask::Activate()
{
	if (this->WaitForInputID == EAbilityInputID::None)
	{
		UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(Ability);
		this->WaitForInputID = BaseAbility->AbilityInputID;
	}

	if (this->WaitForInputID != EAbilityInputID::None)
	{
		UInputComponent* InputComponent = this->GetAvatarActor()->InputComponent;
		if (IsValid(InputComponent))
		{
			UEnum* AbilityIDEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAbilityInputID"), true);
			FString AbilityIDName = AbilityIDEnum->GetNameStringByIndex(static_cast<uint8>(this->WaitForInputID));

			this->ReleaseBinding = FInputActionBinding(FName(*AbilityIDName), IE_Released);
			this->ReleaseBinding.bExecuteWhenPaused = true;
			this->ReleaseBinding.ActionDelegate.BindDelegate(this, &UWaitForSpecificInputAbilityTask::OnActionRelease);
			this->ReleaseBinding = InputComponent->AddActionBinding(this->ReleaseBinding);

			this->PressBinding = FInputActionBinding(FName(*AbilityIDName), IE_Pressed);
			this->PressBinding.bExecuteWhenPaused = true;
			this->PressBinding.ActionDelegate.BindDelegate(this, &UWaitForSpecificInputAbilityTask::OnActionPress);
			this->PressBinding = InputComponent->AddActionBinding(this->PressBinding);

			if (this->bTestInitialState)
			{
				bool bIsInputPressed = Ability->GetCurrentAbilitySpec()->InputPressed;
				if (bIsInputPressed)
				{
					this->OnActionPress();
				}
				else if (!bIsInputPressed)
				{
					this->OnActionRelease();
				}
			}
		}
		else
		{
			EndTask();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WaitForSpecificInputAbilityTask must have an AbilityInput != None"));
	}
}

void UWaitForSpecificInputAbilityTask::OnActionRelease()
{
	this->bWasReleased = true;
	this->bWasPressed = false;
}

void UWaitForSpecificInputAbilityTask::OnActionPress()
{
	this->bWasReleased = false;
	this->bWasPressed = true;
}

void UWaitForSpecificInputAbilityTask::OnDestroy(bool bInOwnerFinished)
{
	UInputComponent* InputComponent = this->GetAvatarActor()->InputComponent;
	if (IsValid(InputComponent))
	{
		InputComponent->RemoveActionBindingForHandle(this->PressBinding.GetHandle());
		InputComponent->RemoveActionBindingForHandle(this->ReleaseBinding.GetHandle());
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UWaitForSpecificInputAbilityTask::TickTask(float DeltaTime)
{
	if (this->bWasReleased)
	{
		this->bWasReleased = false;
		this->OnReleaseNoticed();
	}
	if (this->bWasPressed)
	{
		this->bWasPressed = false;
		this->OnPressNoticed();
	}
}

void UWaitForSpecificInputAbilityTask::OnReleaseNoticed()
{
	if (this->bDoesEndOnRelease)
	{
		this->EndTask();
	}
}

void UWaitForSpecificInputAbilityTask::OnPressNoticed()
{
	if (this->bDoesEndOnPress)
	{
		this->EndTask();
	}
}

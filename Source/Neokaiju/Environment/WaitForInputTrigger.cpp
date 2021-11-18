// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitForInputTrigger.h"
#include "AbilitySystemComponent.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "AbilitySystemInterface.h"

void AWaitForInputTrigger::OnBeginTrigger(AActor* TriggeringActorValue)
{
	this->TriggeringActor = TriggeringActorValue;
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(this->TriggeringActor);
	if (ASCInterface)
	{
		this->TriggeringASC = ASCInterface->GetAbilitySystemComponent();

		this->DisableCharacterInputEffectHandle = this->TriggeringASC->ApplyGameplayEffectToSelf(this->DisableCharacterInputEffectClass.GetDefaultObject(), 1, this->TriggeringASC->MakeEffectContext());
	}

	if (IsValid(this->TriggeringActor) && IsValid(this->TriggeringActor->InputComponent))
	{
		this->WaitForInputBinding = this->TriggeringActor->InputComponent->BindAction(this->WaitForInputName, EInputEvent::IE_Pressed, this, &AWaitForInputTrigger::WaitForInputPressed);
	}
}

void AWaitForInputTrigger::WaitForInputPressed()
{
	if (IsValid(this->TriggeringActor) && IsValid(this->TriggeringActor->InputComponent) && this->WaitForInputBinding.IsValid())
	{
		this->TriggeringActor->InputComponent->RemoveActionBindingForHandle(this->WaitForInputBinding.GetHandle());
	}

	if (this->ReenableCharacterInputAfterWaitForInputPressedDelay > 0)
	{
		FTimerHandle EnableInputTimerHandler;
		this->GetWorld()->GetTimerManager().SetTimer(EnableInputTimerHandler, this, &AWaitForInputTrigger::EnableCharacterInput, this->ReenableCharacterInputAfterWaitForInputPressedDelay);
	}
	else
	{
		this->EnableCharacterInput();
	}
}

void AWaitForInputTrigger::EnableCharacterInput()
{
	if (IsValid(this->TriggeringASC))
	{
		if (this->DisableCharacterInputEffectHandle.IsValid())
		{
			this->TriggeringASC->RemoveActiveGameplayEffect(this->DisableCharacterInputEffectHandle);
			this->DisableCharacterInputEffectHandle.Invalidate();
		}
	}
}

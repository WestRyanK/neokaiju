// Fill out your copyright notice in the Description page of Project Settings.


#include "OverflowAbility.h"

UOverflowAbility::UOverflowAbility()
{
	this->bDoesAbilityActivateOnGranted = true;
}

float UOverflowAbility::GetCurrentAmount() const
{
	return this->CurrentAmount;
}

void UOverflowAbility::SetCurrentAmount(float CurrentAmountValue)
{
	this->CurrentAmount = CurrentAmountValue;

	AB_LOG(TEXT("Overflow %f/%f"), this->CurrentAmount, this->OverflowAmount);
	while (this->CurrentAmount >= this->OverflowAmount)
	{
		this->CurrentAmount -= this->OverflowAmount;
		for (TSubclassOf<UGameplayEffect> OverflowEffect : this->OverflowEffects)
		{
			if (IsValid(OverflowEffect))
			{
				this->GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(OverflowEffect.GetDefaultObject(), 1, this->GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());
			}
		}
	}
}

void UOverflowAbility::AddToCurrentAmount(float AmountToAdd)
{
	this->SetCurrentAmount(this->GetCurrentAmount() + AmountToAdd);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameplayAbility.h"
#include "TimerManager.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
	this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UBaseGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (this->bDoesAbilityActivateOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}


void UBaseGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (this->InstancingPolicy == EGameplayAbilityInstancingPolicy::InstancedPerActor && this->bRetriggerInstancedAbility)
	{
		bool success = this->GetAbilitySystemComponentFromActorInfo()->TryActivateAbility(this->CurrentSpecHandle);
	}
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (bWasCancelled && this->bDoesAbilityRestartWhenCancelled)
	{
		this->GetWorld()->GetTimerManager().SetTimer(this->RestartAbilityTimerHandle, this, &UBaseGameplayAbility::RestartAbility, this->RestartAttemptInterval, true);
	}
}

void UBaseGameplayAbility::RestartAbility()
{
	UAbilitySystemComponent* ASC = this->GetAbilitySystemComponentFromActorInfo();
	if (IsValid(ASC))
	{
		bool bWasSuccess = ASC->TryActivateAbility(this->CurrentSpecHandle);
		if (bWasSuccess)
		{
			this->GetWorld()->GetTimerManager().ClearTimer(this->RestartAbilityTimerHandle);
		}
	}
}

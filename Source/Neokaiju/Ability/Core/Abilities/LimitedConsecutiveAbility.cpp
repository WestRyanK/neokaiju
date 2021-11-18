// Fill out your copyright notice in the Description page of Project Settings.


#include "LimitedConsecutiveAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void ULimitedConsecutiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* ASC = this->GetAbilitySystemComponentFromActorInfo();
	ASC->RegisterGenericGameplayTagEvent().RemoveAll(this);
	ASC->RegisterGenericGameplayTagEvent().AddUObject(this, &ULimitedConsecutiveAbility::OnTagChanged);
}

void ULimitedConsecutiveAbility::OnTagChanged(const FGameplayTag Tag, int32 TagCount)
{
	if (this->ResetConsecutiveAbilitiesTags.HasTagExact(Tag) && this->DoesOwnerHaveResetTags())
	{
		this->ResetConsecutiveCount();
	}
}

void ULimitedConsecutiveAbility::ResetConsecutiveCount()
{
	this->ConsecutiveAbilitiesCount = 0;
}

bool ULimitedConsecutiveAbility::DoesOwnerHaveResetTags() const
{
	bool bHasResetTag = this->GetAbilitySystemComponentFromActorInfo()->HasAnyMatchingGameplayTags(this->ResetConsecutiveAbilitiesTags);

	return bHasResetTag;
}

bool ULimitedConsecutiveAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// This is important! Super checks to see if cooldown is still happening! If we don't have this, it will ignore cooldown.
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (this->DoesOwnerHaveResetTags())
	{
		return true;
	}
	else
	{
		bool bAreAbilitiesRemaining = (this->ConsecutiveAbilitiesCount < this->MaxConsecutiveAbilities);
		return bAreAbilitiesRemaining;
	}
}

bool ULimitedConsecutiveAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// This is important! 
	// Super checks CanActivateAbility and consequently checks Cooldown.
	if (!Super::CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return false;
	}

	if (!this->DoesOwnerHaveResetTags() || this->bDoesCountIncludeWhenOwnerHasResetTags)
	{
		this->ConsecutiveAbilitiesCount++;
	}
	return true;
}


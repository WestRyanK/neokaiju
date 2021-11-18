// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChangeThreshold.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterEventSubsystem.h"
#include "Neokaiju/Core/DamageableAttributeSet.h"

UDeathAbility::UDeathAbility()
{
}

void UDeathAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	this->CurrentActorInfo = ActorInfo;
	this->CurrentSpec = Spec;
	ActorInfo->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UDamageableAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UDeathAbility::OnHealthChanged);
}

void UDeathAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AB_LOG(TEXT("You DIED!!!!!!1"));
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->CurrentActorInfo->AvatarActor.Get());
	if (IsValid(Character))
	{
		if (IsValid(this->DeathEffect))
		{
			Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(this->DeathEffect.GetDefaultObject(), 1, Character->GetAbilitySystemComponent()->MakeEffectContext());
		}
		this->OnDeath();
		UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
		KaijuCharacterEventSubsystem->KaijuDied();
	}
}

void UDeathAbility::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.0f)
	{
		this->CurrentActorInfo->AbilitySystemComponent->TryActivateAbility(this->CurrentSpec.Handle, true);
	}
}

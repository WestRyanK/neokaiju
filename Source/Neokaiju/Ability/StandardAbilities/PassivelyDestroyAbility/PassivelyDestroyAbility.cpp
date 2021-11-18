// Fill out your copyright notice in the Description page of Project Settings.


#include "PassivelyDestroyAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/Core/Tasks/RepeatInfiniteAbilityTask.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemInterface.h"


UPassivelyDestroyAbility::UPassivelyDestroyAbility()
{
	this->bDoesAbilityActivateOnGranted = true;
}

void UPassivelyDestroyAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	this->Character = Cast<AKaijuCharacter>(this->GetAvatarActorFromActorInfo());

	if (IsValid(this->Character))
	{
		// We want to check for overlaps so we immediately react when an actor is overlapped.
		this->Character->PassivelyDestroyHitBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &UPassivelyDestroyAbility::HitBoxOverlap);

		// See note on TimeBetweenPassivelyDestroyBackupTests.
		URepeatInfiniteAbilityTask* Task = URepeatInfiniteAbilityTask::RepeatAction(this, this->TimeBetweenPassivelyDestroyBackupTests);
		Task->OnPerformAction.AddUniqueDynamic(this, &UPassivelyDestroyAbility::BackupTick);
		Task->ReadyForActivation();
	}
}

void UPassivelyDestroyAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(this->Character))
	{
		this->Character->PassivelyDestroyHitBox->OnComponentBeginOverlap.RemoveDynamic(this, &UPassivelyDestroyAbility::HitBoxOverlap);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPassivelyDestroyAbility::HitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool bWasPassivelyDestroyed = this->TryPassivelyDestroy(OtherActor);
}

void UPassivelyDestroyAbility::BackupTick(int32 ActionNumber)
{
	TSet<AActor*> OverlappingActors;
	this->Character->PassivelyDestroyHitBox->GetOverlappingActors(OverlappingActors);
	OverlappingActors.Remove(this->Character);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		bool bWasPassivelyDestroyed = this->TryPassivelyDestroy(OverlappingActor);
	}
}

bool UPassivelyDestroyAbility::TryPassivelyDestroy(AActor* ActorToPassivelyDestroy)
{
	if (ActorToPassivelyDestroy != this->GetAvatarActorFromActorInfo())
	{
		IAbilitySystemInterface* ASCActor = Cast<IAbilitySystemInterface>(ActorToPassivelyDestroy);
		if (ASCActor)
		{
			if (IsValid(this->PassivelyDestroyEffect))
			{
				FActiveGameplayEffectHandle Handle = this->CurrentActorInfo->AbilitySystemComponent->ApplyGameplayEffectToTarget(
					this->PassivelyDestroyEffect.GetDefaultObject(),
					ASCActor->GetAbilitySystemComponent(),
					1);
				if (Handle.WasSuccessfullyApplied())
				{
					this->OnPassivelyDestroyed();
					return true;
				}
			}
		}
	}
	return false;
}

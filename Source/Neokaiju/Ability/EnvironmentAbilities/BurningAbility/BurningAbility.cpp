// Fill out your copyright notice in the Description page of Project Settings.


#include "BurningAbility.h"
#include "Neokaiju/Ability/Core/Tasks/RepeatInfiniteAbilityTask.h"
#include "AbilitySystemInterface.h"
#include "DrawDebugHelpers.h"


UBurningAbility::UBurningAbility()
{
	this->bDoesAbilityActivateOnGranted = true;
}

void UBurningAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//int RepeatCount = int(this->BurningTimeSeconds / this->TimeBetweenSpreadAttemptsSeconds);
	float TimeBetweenSpreadAttempts = FMath::FRandRange(this->MinTimeBetweenSpreadAttemptsSeconds, this->MaxTimeBetweenSpreadAttemptsSeconds);
	URepeatInfiniteAbilityTask* RepeatTask = URepeatInfiniteAbilityTask::RepeatAction(this, TimeBetweenSpreadAttempts);
	RepeatTask->OnPerformAction.AddUniqueDynamic(this, &UBurningAbility::SpreadBurn);
	RepeatTask->ReadyForActivation();
}

void UBurningAbility::SpreadBurn(int32 ActionNumber)
{
	if (ActionNumber > 0)
	{
		AB_LOG(TEXT("Burning!"));
		FBox SpreadVolume = this->CurrentActorInfo->AvatarActor->GetComponentsBoundingBox().ExpandBy(this->SpreadDistance);
		FCollisionQueryParams Params = FCollisionQueryParams();
		Params.AddIgnoredActor(this->CurrentActorInfo->AvatarActor.Get());
		TArray<FOverlapResult> Overlaps;
		this->GetWorld()->OverlapMultiByChannel(Overlaps, SpreadVolume.GetCenter(), FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, FCollisionShape::MakeBox(SpreadVolume.GetExtent()), Params);
		DrawDebugBox(this->GetWorld(), SpreadVolume.GetCenter(), SpreadVolume.GetExtent(), FColor::Red, false, 0.1f, 0, 1.0f);

		for (const FOverlapResult Overlap : Overlaps)
		{
			if (FMath::FRand() < this->ProbabilityOfSpreading)
			{
				IAbilitySystemInterface* OverlappedActor = Cast<IAbilitySystemInterface>(Overlap.Actor.Get());
				if (OverlappedActor)
				{
					AB_LOG(TEXT("Spread to %s"), *Overlap.Actor.Get()->GetFName().ToString());
					if (IsValid(this->BurnEffect))
					{
						OverlappedActor->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(this->BurnEffect.GetDefaultObject(), 1, OverlappedActor->GetAbilitySystemComponent()->MakeEffectContext());
					}
				}
			}
		}
	}
}

void UBurningAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(this->BurnedEffect))
	{
		this->CurrentActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(this->BurnedEffect.GetDefaultObject(), 1, this->CurrentActorInfo->AbilitySystemComponent->MakeEffectContext());
	}
	//this->RemoveGrantedByEffect();
	this->CurrentActorInfo->AbilitySystemComponent.Get()->ClearAbility(this->CurrentSpecHandle);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBurningAbility::Finished(int32 ActionNumber)
{
	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
}

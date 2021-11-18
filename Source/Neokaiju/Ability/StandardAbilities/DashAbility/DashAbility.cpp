// Fill out your copyright notice in the Description page of Project Settings.



#include "DashAbility.h"
#include "Neokaiju/Ability/StandardAbilities/DashAbility/DashAbilityTask.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Curves/CurveFloat.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

UDashAbility::UDashAbility()
{
	this->bDoesCountIncludeWhenOwnerHasResetTags = true;
}

void UDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AB_LOG(TEXT("Dashing: #%d"), this->ConsecutiveAbilitiesCount);

	this->Character = Cast<AKaijuCharacter>(GetAvatarActorFromActorInfo());
	if (IsValid(this->Character))
	{

		this->HitActors.Empty();
		this->ChainedDashCount++;

		this->OnDashStart(this->Character->GetPrimaryAimDirection(), this->ChainedDashCount);

		UDashAbilityTask* DashAbilityTask = UDashAbilityTask::Dash(
			this,
			this->Character->GetPrimaryAimDirection(),
			this->Character->GetDashDistance(),
			this->Character->GetDashDuration(),
			this->bDoesAllowGlancingHitRedirect,
			this->MinGlancingHitRedirectAngle,
			this->bDoesAllowOffCenterHitSliding,
			this->MaxOffCenterClipHitSize,
			this->DashPositionOverTime,
			this->bContinueVelocityAfterDash);
		DashAbilityTask->OnCompleted.AddDynamic(this, &UDashAbility::Completed);
		DashAbilityTask->OnDashImpeded.AddDynamic(this, &UDashAbility::DashImpeded);
		DashAbilityTask->OnDashHit.AddDynamic(this, &UDashAbility::DashHit);
		DashAbilityTask->OnTick.AddDynamic(this, &UDashAbility::DashTick);
		DashAbilityTask->ReadyForActivation();
	}
}

void UDashAbility::Completed()
{
	AB_LOG(TEXT("Dash Ended"));
	this->OnDashComplete();

	for (TSubclassOf<UGameplayEffect> OnCompletedDashEffect : this->OnCompletedDashEffects)
	{
		if (IsValid(OnCompletedDashEffect))
		{
			this->ApplyGameplayEffectToOwner(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, OnCompletedDashEffect.GetDefaultObject(), 1);
		}
	}

	this->EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDashAbility::ResetConsecutiveCount()
{
	Super::ResetConsecutiveCount();
	this->ChainedDashCount = 0;
}

void UDashAbility::DashHit(FVector CharacterVelocity, FVector ImpactNormal, AActor* HitActor)
{
	TSet<AActor*> HitByBoxActors;
	this->Character->PassivelyDestroyHitBox->GetOverlappingActors(HitByBoxActors);
	for (AActor* HitByBoxActor : HitByBoxActors)
	{
		if (!this->HitActors.Contains(HitByBoxActor) && HitByBoxActor != this->Character)
		{
			this->HitActors.Add(HitByBoxActor);

			IAbilitySystemInterface* ASCActor = Cast<IAbilitySystemInterface>(HitByBoxActor);
			if (ASCActor)
			{
				for (TSubclassOf<UGameplayEffect> OnDashHitEffect : this->OnDashHitEffects)
				{
					if (IsValid(OnDashHitEffect))
					{
						// Attempt to use the OnDashHitEffect on anything we can.
						AB_LOG(TEXT("Hit: %s"), *HitActor->GetName());
						this->GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToTarget(OnDashHitEffect.GetDefaultObject(), ASCActor->GetAbilitySystemComponent(), 1);
					}
				}
			}
		}
	}


	if (this->bCanChainSuccessfulDashes)
	{
		this->ConsecutiveAbilitiesCount = 0;
	}

	bool bWasImpeded = (IsValid(HitActor));
	this->OnDashHit(bWasImpeded);
}

void UDashAbility::DashImpeded(FVector CharacterVelocity, FVector ImpactNormal, AActor* ActorHit)
{
	AB_LOG(TEXT("Dash Impeded"));
	this->OnDashImpede();
	if (IsValid(this->Character))
	{
		if (!this->bIsBounceVelocityBasedOnDashVelocity)
		{
			CharacterVelocity.Normalize();
		}

		FVector BounceVelocity = CharacterVelocity.MirrorByVector(ImpactNormal) * this->BounceVelocityMultiplier + FVector(0, 0, this->BounceVerticalVelocityAddition);
		this->Character->LaunchCharacter(BounceVelocity, true, true);
	}
	for (TSubclassOf<UGameplayEffect> OnImpededDashEffect : this->OnImpededDashEffects)
	{
		if (IsValid(OnImpededDashEffect))
		{
			this->ApplyGameplayEffectToOwner(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, OnImpededDashEffect.GetDefaultObject(), 1);
		}
	}

	if (this->bCanChainSuccessfulDashes)
	{
		this->ConsecutiveAbilitiesCount = this->MaxConsecutiveAbilities;
	}

	this->EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDashAbility::DashTick(float DeltaTime, FVector DashDirection)
{
	this->OnDashTick(DeltaTime, DashDirection);
}

void UDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	this->OnDashComplete();
}


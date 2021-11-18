// Fill out your copyright notice in the Description page of Project Settings.


#include "FixedSwipeAbility.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/Core/Tasks/TickDurationAbilityTask.h"
#include "DrawDebugHelpers.h"


UFixedSwipeAbility::UFixedSwipeAbility()
{

}

void UFixedSwipeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	this->Character = Cast<AKaijuCharacter>(ActorInfo->AvatarActor.Get());

	if (IsValid(this->Character))
	{
		if (this->bDoesFreezeInAir)
		{
			this->Character->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;
		}

		this->Character->TailHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		this->HitActors.Empty();
		this->Character->TailHitBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &UFixedSwipeAbility::OnTailHit);

		this->SwipeDegreesPerSecond = this->SwipeDegrees / this->SwipeDuration;
		this->TotalDegreesRotated = 0.0f;
		this->InitialFacingDirection = this->Character->KaijuMovement->GetFacingDirection();

		FRotator InitialSwipeRotation = FRotator(-(180.0f - this->SwipeDegrees * 0.5f), 0, 0);
		this->Character->KaijuMovement->RotateCharacterRelative(InitialSwipeRotation, false);

		this->TickTask = UTickDurationAbilityTask::TickForDuration(this, this->SwipeDuration);
		TickTask->OnDurationTick.AddUniqueDynamic(this, &UFixedSwipeAbility::OnSwipeTick);
		TickTask->OnDurationFinished.AddUniqueDynamic(this, &UFixedSwipeAbility::OnSwipeFinished);
		TickTask->ReadyForActivation();

	}
	//this->PreviousDeltaAngles.Empty();
}

void UFixedSwipeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(this->Character))
	{
		if (this->bDoesFreezeInAir)
		{
			this->Character->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;
		}
		this->Character->KaijuMovement->SetCharacterForward(USafeDirectionUtility::GetSafeForwardDirectionFromFacing(this->InitialFacingDirection));
		this->Character->TailHitBox->OnComponentBeginOverlap.RemoveDynamic(this, &UFixedSwipeAbility::OnTailHit);

		this->Character->TailHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFixedSwipeAbility::OnTailHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this->GetAvatarActorFromActorInfo())
	{
		if (!this->HitActors.Contains(OtherActor)) // Only hit each actor once per swipe. We could hit them multiple times because they could have multiple components.
		{
			this->HitActors.Add(OtherActor);
			AB_LOG(TEXT("Tail hit something"));

			IAbilitySystemInterface* ASCActor = Cast<IAbilitySystemInterface>(OtherActor);
			if (ASCActor)
			{
				if (IsValid(this->SwipeDamageEffect))
				{
					this->CurrentActorInfo->AbilitySystemComponent->ApplyGameplayEffectToTarget(this->SwipeDamageEffect.GetDefaultObject(), ASCActor->GetAbilitySystemComponent(), 1);
				}
				this->OnSwipeHit();
			}
		}
	}
}

void UFixedSwipeAbility::OnSwipeFinished(float DeltaTime)
{
	AB_LOG(TEXT("Done swiping"));
	TickTask->EndTask();
	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
}

bool UFixedSwipeAbility::ShouldStopSwiping()
{
	return TotalDegreesRotated >= SwipeDegrees;
}

void UFixedSwipeAbility::OnSwipeTick(float DeltaTime)
{
	if (IsValid(this->Character))
	{
		if (this->ShouldStopSwiping())
		{
			this->OnSwipeFinished(DeltaTime);
		}
		else
		{
			float DeltaSwipeDegrees = this->SwipeDegreesPerSecond * DeltaTime;
			this->TotalDegreesRotated += DeltaSwipeDegrees;
			FRotator SwipeRotation = FRotator(-DeltaSwipeDegrees, 0, 0);
			this->Character->KaijuMovement->RotateCharacterRelative(SwipeRotation, false);
		}
	}
}

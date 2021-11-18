// Fill out your copyright notice in the Description page of Project Settings.


#include "LandOnFeetSwipeAbility.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/Core/Tasks/TickDurationAbilityTask.h"
#include "DrawDebugHelpers.h"


ULandOnFeetSwipeAbility::ULandOnFeetSwipeAbility()
{

}

void ULandOnFeetSwipeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		this->HitActors.Empty();
		this->Character->TailHitBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ULandOnFeetSwipeAbility::OnTailHit);

		this->SwipeDegreesPerSecond = 360.0f / this->MaxSwipeDuration;
		this->InitialCharacterRotation = this->Character->CharacterRotationPoint->GetComponentRotation();
		this->TotalDegreesRotated = 0.0f;

		this->TickTask = UTickDurationAbilityTask::TickForDuration(this, this->MaxSwipeDuration * 2);
		TickTask->OnDurationTick.AddUniqueDynamic(this, &ULandOnFeetSwipeAbility::OnSwipeTick);
		TickTask->OnDurationFinished.AddUniqueDynamic(this, &ULandOnFeetSwipeAbility::OnSwipeFinished);
		TickTask->ReadyForActivation();

		this->PredictLandingSpot();
	}
	this->PreviousDeltaAngles.Empty();
}

void ULandOnFeetSwipeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(this->Character))
	{
		if (this->bDoesFreezeInAir)
		{
			this->Character->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Falling;
		}
		this->Character->TailHitBox->OnComponentBeginOverlap.RemoveDynamic(this, &ULandOnFeetSwipeAbility::OnTailHit);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULandOnFeetSwipeAbility::OnTailHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ULandOnFeetSwipeAbility::OnSwipeFinished(float DeltaTime)
{
	AB_LOG(TEXT("Done swiping"));
	TickTask->EndTask();
	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
}

bool ULandOnFeetSwipeAbility::ShouldStopSwiping()
{
	if (bIsFixedSwipeLength)
	{
		return TotalDegreesRotated >= MinSwipeDegrees;
	}
	else
	{
		FVector RightVector = USafeDirectionUtility::GetSafeRightVector(this->Character->CharacterRotationPoint->GetRightVector());
		FVector LandingForwardVector = RightVector ^ this->PredictedLandingSpot.ImpactNormal;
		FVector CurrentForwardVector = this->Character->CharacterRotationPoint->GetForwardVector();
		float LandingForwardAngle = USafeDirectionUtility::GetAngleOfVectorAroundYAxis(LandingForwardVector);
		float CurrentForwardAngle = USafeDirectionUtility::GetAngleOfVectorAroundYAxis(CurrentForwardVector);

		//AB_LOG(TEXT("Current Angle %f, Landing Angle %f"), CurrentForwardAngle, LandingForwardAngle);
		float DeltaAngle = FMath::Abs(FMath::FindDeltaAngleDegrees(LandingForwardAngle, CurrentForwardAngle));
		this->PreviousDeltaAngles.Add(DeltaAngle);

		bool bIsNearLandingAngle = (DeltaAngle < 5.0f);
		bool bWasMinDegreesRotatedReached = (this->TotalDegreesRotated >= this->MinSwipeDegrees);
		//AB_LOG(TEXT("DeltaAbs: %f, Should stop? %d"), DeltaAngle, bIsNearLandingAngle);

		if (this->PreviousDeltaAngles.Num() >= 3) // We don't want to cancel too early.
		{
			bool bWasInflectionPointReached = (this->PreviousDeltaAngles.Last(2) > this->PreviousDeltaAngles.Last(1) && this->PreviousDeltaAngles.Last(0) > this->PreviousDeltaAngles.Last(1));
			return (bWasInflectionPointReached || bIsNearLandingAngle) && bWasMinDegreesRotatedReached; // We want to stop either if the current angle is close enough to the landing angle, or even if it wasn't close enough, but we overshot it.
		}
		else
		{
			return false;
		}
	}
}

void ULandOnFeetSwipeAbility::PredictLandingSpot()
{
	FPredictProjectilePathParams PathParams;
	PathParams.LaunchVelocity = this->Character->GetVelocity();
	PathParams.ActorsToIgnore.Add(this->Character);
	PathParams.OverrideGravityZ = this->Character->GetCharacterMovement()->GetGravityZ();
	PathParams.ProjectileRadius = this->Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	PathParams.StartLocation = this->Character->GetActorLocation();
	PathParams.bTraceComplex = true;
	PathParams.SimFrequency = 10.0f;
	PathParams.bTraceWithCollision = true;
#if !UE_BUILD_SHIPPING // Don't draw when shipping
	if (this->bShouldDebugDrawPredictedLanding)
	{
		PathParams.DrawDebugType = EDrawDebugTrace::ForDuration;
		PathParams.DrawDebugTime = 0.5f;
	}
#endif
	PathParams.MaxSimTime = 2.0f;
	FPredictProjectilePathResult PathResult;
	bool bDidPathHit = UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
	if (bDidPathHit)
	{
		this->PredictedLandingSpot = PathResult.HitResult;
	}
	else
	{
		this->PredictedLandingSpot = FHitResult(nullptr, nullptr, FVector::ZeroVector, FVector::UpVector);
	}
}

void ULandOnFeetSwipeAbility::OnSwipeTick(float DeltaTime)
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
			this->Character->CharacterRotationPoint->AddLocalRotation(SwipeRotation, true);
		}
	}
}


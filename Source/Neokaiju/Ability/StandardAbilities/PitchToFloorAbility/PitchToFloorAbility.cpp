// Fill out your copyright notice in the Description page of Project Settings.




#include "PitchToFloorAbility.h"
#include "Neokaiju/Ability/Core/Tasks/TickDurationAbilityTask.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuMovementComponent.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPitchToFloorAbility::UPitchToFloorAbility()
{

}

void UPitchToFloorAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		UTickDurationAbilityTask* Task = UTickDurationAbilityTask::TickForDuration(this);
		Task->OnDurationTick.AddUniqueDynamic(this, &UPitchToFloorAbility::Tick);
		Task->ReadyForActivation();
	}
}

void UPitchToFloorAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPitchToFloorAbility::Tick(float DeltaTime)
{
	if (IsValid(this->Character))
	{
		FVector CurrentForwardDirection = USafeDirectionUtility::GetSafeForwardDirectionFromFacing(this->Character->KaijuMovement->GetFacingDirection());
		float CurrentAdjustSpeed = this->AdjustPitchSpeed;
		if (CurrentForwardDirection != this->PreviousForwardDirection && this->PreviousForwardDirection != FVector::ZeroVector)
		{
			CurrentAdjustSpeed = 0.0f;
		}

		FVector  FloorNormal;
		FHitResult FloorHitResult = this->Character->GetCharacterMovement()->CurrentFloor.HitResult;
		if (this->ShouldPitchToSurface(FloorHitResult.Actor.Get()))
		{
			FloorNormal = this->Character->GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal;
		}
		else
		{
			FloorNormal = FVector::UpVector;
		}

		FVector RightVector = USafeDirectionUtility::GetSafeRightVector(this->Character->CharacterRotationPoint->GetRightVector());
		FVector Forward = RightVector ^ FloorNormal;

		this->Character->KaijuMovement->SetCharacterForward(
			Forward,
			CurrentForwardDirection,
			FVector::ZeroVector,
			CurrentAdjustSpeed,
			DeltaTime);

		this->PreviousForwardDirection = USafeDirectionUtility::GetSafeForwardDirectionFromFacing(this->Character->KaijuMovement->GetFacingDirection());
	}
}

bool UPitchToFloorAbility::ShouldPitchToSurface(AActor* Surface) const
{
	if (this->bShouldAllSurfacesBeHorizontal)
	{
		return false;
	}

	return  !this->bShouldBreakableStructureSurfacesBeHorizontal || !IsValid(Cast<ABreakableStructure>(Surface));
}

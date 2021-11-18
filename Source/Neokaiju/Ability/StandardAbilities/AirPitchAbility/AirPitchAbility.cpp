// Fill out your copyright notice in the Description page of Project Settings.


#include "AirPitchAbility.h"
#include "Neokaiju/Ability/Core/Tasks/TickDurationAbilityTask.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UAirPitchAbility::UAirPitchAbility()
{

}

void UAirPitchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		Task->OnDurationTick.AddUniqueDynamic(this, &UAirPitchAbility::Tick);
		Task->ReadyForActivation();
	}
}

void UAirPitchAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UAirPitchAbility::Tick(float DeltaTime)
{
	if (IsValid(this->Character))
	{
		FVector Velocity = this->Character->GetCharacterMovement()->Velocity;
		FVector ForwardDirection = USafeDirectionUtility::GetSafeForwardDirectionFromFacing(this->Character->KaijuMovement->GetFacingDirection());
		FVector PitchDirection = FVector(ForwardDirection.X, 0, Velocity.Z * this->AirPitchVelocityMultiplier);

		this->Character->KaijuMovement->SetCharacterForward(
			ForwardDirection,
			//PitchDirection,
			ForwardDirection,
			FVector::ZeroVector,
			this->AdjustPitchSpeed,
			DeltaTime);
	}
}


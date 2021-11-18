// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityAlterJumpAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/Core/Tasks/TickDurationAbilityTask.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

void UGravityAlterJumpAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	ACharacter* CharacterActor = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (IsValid(CharacterActor))
	{
		this->OriginalGravityScale = CharacterActor->GetCharacterMovement()->GravityScale;
		AB_LOG(TEXT("Original Gravity: %f"), this->OriginalGravityScale);
	}
	Super::OnAvatarSet(ActorInfo, Spec);
}

void UGravityAlterJumpAbility::ExecuteJump(FVector JumpDirection, float JumpHeight)
{
	float JumpVelocity = this->CalculateVelocityForJumpHeight(JumpHeight, this->JumpPressedGravityScale);
	this->Character->GetCharacterMovement()->GravityScale = this->JumpPressedGravityScale;

	this->Character->GetCharacterMovement()->GetMaxJumpHeight();
	this->Character->LaunchCharacter(JumpVelocity * JumpDirection, false, true);

	UTickDurationAbilityTask* AwaitCharacterFallingTask = UTickDurationAbilityTask::TickForDuration(this);
	AwaitCharacterFallingTask->OnDurationTick.AddUniqueDynamic(this, &UGravityAlterJumpAbility::AwaitCharacterFalling);
	AwaitCharacterFallingTask->ReadyForActivation();

	UAbilityTask_WaitInputRelease* WaitJumpRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitJumpRelease->OnRelease.AddUniqueDynamic(this, &UGravityAlterJumpAbility::OnJumpReleased);
	WaitJumpRelease->ReadyForActivation();
}


void UGravityAlterJumpAbility::AwaitCharacterFalling(float DeltaTime)
{
	FVector CurrentVelocity = this->Character->GetVelocity();
	if (CurrentVelocity.Z <= 0)
	{
		this->Character->GetCharacterMovement()->Velocity.Z = 0;
		this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
	}
}

void UGravityAlterJumpAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	this->Character->GetCharacterMovement()->GravityScale = this->OriginalGravityScale;
	AB_LOG(TEXT("Set Original Gravity to: %f"), this->OriginalGravityScale);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGravityAlterJumpAbility::OnJumpReleased(float TimeHeld)
{
	AB_LOG(TEXT("Jump Released"));
	this->Character->GetCharacterMovement()->GravityScale = this->JumpReleasedGravityScale;
}

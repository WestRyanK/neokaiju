// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpAlongCurveAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/StandardAbilities/JumpAbility/JumpAlongCurveAbilityTask.h"
#include "GameFramework/CharacterMovementComponent.h"

void UJumpAlongCurveAbility::ExecuteJump(FVector JumpDirection, float JumpHeight)
{
	// Pause gravity and previous Z velocity so jump can override that.
	// Set movementmode to falling so character can get off ground.
	this->OriginalCharacterGravity = this->Character->GetCharacterMovement()->GravityScale;
	this->Character->GetCharacterMovement()->GravityScale = 0.0f;
	this->Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	FVector OldVelocity = this->Character->GetCharacterMovement()->Velocity;
	this->Character->GetCharacterMovement()->Velocity = FVector(OldVelocity.X, OldVelocity.Y, 0);

	UJumpAlongCurveAbilityTask* JumpAlongCurveTask = UJumpAlongCurveAbilityTask::JumpAlongCurve(
		this,
		JumpDirection,
		JumpHeight,
		this->JumpPositionOverTimeDuration,
		this->JumpPositionOverTime);
	JumpAlongCurveTask->OnCompleted.AddUniqueDynamic(this, &UJumpAlongCurveAbility::OnJumpAlongCurveComplete);
	JumpAlongCurveTask->ReadyForActivation();
}

void UJumpAlongCurveAbility::OnJumpAlongCurveComplete()
{
	this->Character->GetCharacterMovement()->GravityScale = this->OriginalCharacterGravity;
	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
}

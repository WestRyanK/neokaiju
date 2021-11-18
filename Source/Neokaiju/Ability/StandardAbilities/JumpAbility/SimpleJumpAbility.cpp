// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleJumpAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void USimpleJumpAbility::ExecuteJump(FVector JumpDirection, float JumpHeight)
{
	float JumpVelocity = this->CalculateVelocityForJumpHeight(JumpHeight);

	this->Character->LaunchCharacter(JumpVelocity * JumpDirection, false, true);
	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
}

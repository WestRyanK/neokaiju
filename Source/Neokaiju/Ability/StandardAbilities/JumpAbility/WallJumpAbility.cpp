// Fill out your copyright notice in the Description page of Project Settings.


#include "WallJumpAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuMovementComponent.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "GameFramework/PawnMovementComponent.h"

void UWallJumpAbility::ExecuteJump(FVector JumpDirection, float JumpHeight)
{
	this->Character->KaijuMovement->FlipFacingDirection();
	FVector Direction = USafeDirectionUtility::GetSafeForwardDirectionFromFacing(this->Character->KaijuMovement->GetFacingDirection()) * HorizontalVelocity;
	UPawnMovementComponent* MoveComp = this->Character->GetMovementComponent();
	MoveComp->Velocity = Direction;

	UGravityAlterJumpAbility::ExecuteJump(JumpDirection, JumpHeight);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/StandardAbilities/JumpAbility/BaseJumpAbility.h"
#include "JumpAlongCurveAbility.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API UJumpAlongCurveAbility : public UBaseJumpAbility
{
	GENERATED_BODY()

public:
	// Using this curve changes the jump from simply applying an initial force and letting physics control the rest of the motion
	// to a jump where the position changes through time according to the curve.
	// Note that the jump will only control the vertical position of the character and will be cancelled if the character hits anything while jumping.
	// The scale of the jump is still controlled by the character's jump height.
	// The curve only controls the first half of the jump up to the jump's apex. After that, physics/gravity takes over. This is required so that 
	// falling off ledges behaves the same as falling after a jump.
	// Required curve properties:
	// X Axis measures time from 0 to 1:
	// 0 - Jump has not started
	// 1 - Jump has reached the top/apex. 
	// Y Axis measures progress along jump path from 0 to 1:
	// 0 - Jump start position
	// 1 - Jump end position (JumpHeight)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability | Jump")
		UCurveFloat* JumpPositionOverTime;

	// If JumpPositionOverTime is not null, the character's jump will be controlled by that curve. When it is controlled by that curve,
	// the position of the character will be set according to the curve for this amount of time.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability | Jump")
		float JumpPositionOverTimeDuration;

protected:
	UFUNCTION()
		void OnJumpAlongCurveComplete();

	virtual void ExecuteJump(FVector JumpDirection, float JumpHeight);

	float OriginalCharacterGravity;
};

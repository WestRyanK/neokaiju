// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/StandardAbilities/JumpAbility/GravityAlterJumpAbility.h"
#include "WallJumpAbility.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UWallJumpAbility : public UGravityAlterJumpAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability | Jump")
		float HorizontalVelocity = 500.0f;

protected:

	virtual void ExecuteJump(FVector JumpDirection, float JumpHeight);
};

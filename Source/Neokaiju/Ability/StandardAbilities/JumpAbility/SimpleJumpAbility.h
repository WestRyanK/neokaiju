// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/StandardAbilities/JumpAbility/BaseJumpAbility.h"
#include "SimpleJumpAbility.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API USimpleJumpAbility : public UBaseJumpAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ExecuteJump(FVector JumpDirection, float JumpHeight);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "BaseAbilityTask.generated.h"

class UBaseGameplayAbility;

#define ABTASK_LOG(Format, ...) \
	if (Cast<UBaseGameplayAbility>(this->Ability)->bIsAbilityDebugEnabled) { \
		UE_LOG(LogTemp, Display, Format, ##__VA_ARGS__); \
	}

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UBaseAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
};

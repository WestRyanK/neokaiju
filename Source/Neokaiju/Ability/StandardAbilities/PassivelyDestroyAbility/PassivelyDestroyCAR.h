// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCustomApplicationRequirement.h"
#include "PassivelyDestroyCAR.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UPassivelyDestroyCAR : public UGameplayEffectCustomApplicationRequirement
{
	GENERATED_BODY()
	
public:
	virtual bool CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect, const FGameplayEffectSpec& Spec, UAbilitySystemComponent* TargetASC) const;
};

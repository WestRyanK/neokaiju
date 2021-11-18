// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCustomApplicationRequirement.h"
#include "BaseDamageCAR.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UBaseDamageCAR : public UGameplayEffectCustomApplicationRequirement
{
	GENERATED_BODY()

public:
	virtual bool CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect, const FGameplayEffectSpec& Spec, UAbilitySystemComponent* TargetASC) const;

	virtual float GetMinimumLevelToDamage(UAbilitySystemComponent* TargetASC) const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTag ResistDamageTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float OverrideResistKaijuLevel = 4;
	
};

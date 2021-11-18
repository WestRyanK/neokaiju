// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCustomApplicationRequirement.h"
#include "Neokaiju/Ability/Core/BaseDamageCAR.h"
#include "LaserDamageCAR.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API ULaserDamageCAR : public UBaseDamageCAR
{
	GENERATED_BODY()
	
public:
	virtual float GetMinimumLevelToDamage(UAbilitySystemComponent* TargetASC) const;
};

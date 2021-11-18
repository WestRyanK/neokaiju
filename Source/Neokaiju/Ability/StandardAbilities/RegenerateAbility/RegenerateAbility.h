// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/OverflowAbility.h"
#include "RegenerateAbility.generated.h"

class ABreakableStructure;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API URegenerateAbility : public UOverflowAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
		void OnStructureDestroyed(ABreakableStructure* DestroyedStructure);

	virtual void SetCurrentAmount(float CurrentAmountValue);
};

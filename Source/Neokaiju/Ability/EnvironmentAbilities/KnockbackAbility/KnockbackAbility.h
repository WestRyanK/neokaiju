// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "KnockbackAbility.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UKnockbackAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	
	UKnockbackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Knockback")
		float VerticalVelocityMultiplier = 5.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Knockback")
		float HorizontalVelocityMultiplier = 5.0f;
};

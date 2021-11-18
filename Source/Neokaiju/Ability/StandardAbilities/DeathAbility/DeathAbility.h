// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "GameplayEffectTypes.h"
#include "DeathAbility.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API UDeathAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UDeathAbility();

	// The gameplay effect that gets applied to the character when it is dead.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Death")
		class TSubclassOf<class UGameplayEffect> DeathEffect;


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnDeath();

protected:

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

	FGameplayAbilitySpec CurrentSpec;
};

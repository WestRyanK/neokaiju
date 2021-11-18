// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "AirPitchAbility.generated.h"

class AKaijuCharacter;

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UAirPitchAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UAirPitchAbility();

	// How fast the Character should transition between its current pitch and 
	// the desired air pitch. AdjustPitchSpeed of 0 is an instant transition.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float AdjustPitchSpeed = 100.0f;

	// How much the Character pitches forward/back based on vertical velocity.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float AirPitchVelocityMultiplier = 0.02f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:
	AKaijuCharacter* Character;

	UFUNCTION()
		void Tick(float DeltaTime);
};

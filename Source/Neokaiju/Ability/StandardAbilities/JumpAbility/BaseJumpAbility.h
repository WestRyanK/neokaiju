// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/LimitedConsecutiveAbility.h"
#include "BaseJumpAbility.generated.h"

class AKaijuCharacter;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API UBaseJumpAbility : public ULimitedConsecutiveAbility
{
	GENERATED_BODY()
	
public:
	UBaseJumpAbility();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability | Jump")
		float JumpHeightMultiplier = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability | Jump")
		FName FeetCenterSocketName;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnJump(USceneComponent* Component, FName SocketName, int32 ConsecutiveJumpCount);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	// Calculates the initial velocity required to launch the character JumpHeight off the ground based on the amount of gravity.
	// If GravityScale is 0, the value is taken from the Character's current gravity scale, otherwise a custom value can be used.
	float CalculateVelocityForJumpHeight(float JumpHeight, float GravityScale = 0);
	
	virtual void ExecuteJump(FVector JumpDirection, float JumpHeight);

	AKaijuCharacter* Character;
};

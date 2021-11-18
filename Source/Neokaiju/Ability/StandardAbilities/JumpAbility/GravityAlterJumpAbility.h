// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/StandardAbilities/JumpAbility/BaseJumpAbility.h"
#include "GravityAlterJumpAbility.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UGravityAlterJumpAbility : public UBaseJumpAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability | Jump")
		float JumpPressedGravityScale = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability | Jump")
		float JumpReleasedGravityScale = 1.0f;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
protected:
	virtual void ExecuteJump(FVector JumpDirection, float JumpHeight);

	UFUNCTION()
		void AwaitCharacterFalling(float DeltaTime);

	UFUNCTION()
		void OnJumpReleased(float TimeHeld);

	float OriginalGravityScale;
};

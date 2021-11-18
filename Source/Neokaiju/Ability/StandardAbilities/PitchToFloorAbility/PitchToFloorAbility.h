// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "PitchToFloorAbility.generated.h"

class AKaijuCharacter;

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UPitchToFloorAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPitchToFloorAbility();

	// How fast the Character should transition between its current pitch and 
	// the desired ground pitch. AdjustPitchSpeed of 0 is an instant transition.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float AdjustPitchSpeed = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bShouldBreakableStructureSurfacesBeHorizontal;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bShouldAllSurfacesBeHorizontal;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:
	AKaijuCharacter* Character;

	UFUNCTION()
		void Tick(float DeltaTime);

	FVector PreviousForwardDirection = FVector::ZeroVector;

	bool ShouldPitchToSurface(AActor* Surface) const;
};

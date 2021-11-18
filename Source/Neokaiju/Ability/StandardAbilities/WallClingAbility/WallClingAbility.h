// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "WallClingAbility.generated.h"

class AKaijuCharacter;
class UCurveFloat;

/**
 *
 */
UCLASS()
class NEOKAIJU_API UWallClingAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UWallClingAbility();

	// Preferred overMaxDescentSlideSpeed. If this is null, that value will be used instead.
	// This curve represents the MaxDescentSlideSpeed with respect to time, so you can start
	// out with very little sliding and then increase it with time. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UCurveFloat* MaxDescentSlideSpeedOverTime;

	// If MaxDescentSlideSpeedOverTime is null, this value is used.
	// This value represents how fast the Kaiju will slide down a wall while clinging.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MaxDescentSlideSpeed;

	// How fast the Character should transition between its current pitch and 
	// the desired WallCling pitch. AdjustPitchSpeed of 0 is an instant transition.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float AdjustPitchSpeed = 100.0f;

	// If the player has been WallClinging for this many seconds, release grip on the wall
	// and just fall.
	// If value is <= 0 this is ignored.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ReleaseWallClingAfterSeconds;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bCanWallClingOnPhysicsEnabled;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bRequireInputInWallDirectionToStart;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnBeginWallCling();

protected:
	AKaijuCharacter* Character;

	void PitchCharacterToWall(float DeltaTime);

	UFUNCTION()
		void WallClingTick(float DeltaTime);

	UFUNCTION()
		bool CanWallClingOnSurface() const;

	bool IsInputInDirectionOfSurface() const;

	float WallClingStartTime;

	bool GetShouldReleaseFromWall() const;

	float GetCurrentMaxDescentSlideSpeed() const;

	float GetTimeSinceStart() const;

	void SlowCharacterDescent();
};

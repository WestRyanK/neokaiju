// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "TimerManager.h"
#include "ThrownAbility.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API UThrownAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:

	UThrownAbility();

	// The effect to apply to the two actors which collide. The thrown actor and the collided actor.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> ImpactEffect;

	// A map from splash radii to effects to apply at that radius.
	// The center of the splash is offset in the direction of impact by SplashDamageOffset.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TMap<float, TSubclassOf<UGameplayEffect>> SplashDamageEffects;

	// Offsets the SplashDamage area in the direction of impact velocity.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SplashDamageOffset = 400;

	// Often, actors are intersecting with the floor or the kaiju at the time they are thrown,
	// this property gives the actor a grace period ignoring collision for a short time before 
	// enabling collision again.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float InitialNoCollisionPeriod = 0.05f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TEnumAsByte<ECollisionChannel> ThrownCollisionChannel;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:
	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void SetThrownCollision();

	UFUNCTION()
		void EnableCollision();

	FTimerHandle TimerHandle_EnableCollision;
};

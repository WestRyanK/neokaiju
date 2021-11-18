// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/LimitedConsecutiveAbility.h"
#include "DashAbility.generated.h"

class AKaijuCharacter;
class UCurveFloat;

/**
 *
 */
UCLASS()
class NEOKAIJU_API UDashAbility : public ULimitedConsecutiveAbility
{
	GENERATED_BODY()

public:
	UDashAbility();

	// If the dash finishes in the air, should the character continue with
	// the velocity given by the dash, or should the character's velocity be cancelled?
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		bool bContinueVelocityAfterDash = false;

	// The gameplay effect that gets applied to actors which are hit by the dash.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		TArray<TSubclassOf<UGameplayEffect>> OnDashHitEffects;

	// The gameplay effect that gets applied to the ability owner when the dash is impeded.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		TArray<TSubclassOf<UGameplayEffect>> OnImpededDashEffects;

	// The gameplay effect that gets applied to the ability owner when the dash is completed without being impeded.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		TArray<TSubclassOf<UGameplayEffect>> OnCompletedDashEffects;

	// Using this curve changes the dash from a motion with constant velocity from point A to point B
	// to a dash where the velocity changes through time according to the curve.
	// Note that the dash will still be in a straight line, however, with the curve you can make
	// the character slowly speed up as they dash, for example.
	// Required curve properties:
	// X Axis measures time from 0 to 1:
	// 0 - Dash has not started
	// 1 - Dash has completed
	// Y Axis measures progress along dash path from 0 to 1:
	// 0 - Dash start position
	// 1 - Dash end position
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		UCurveFloat* DashPositionOverTime;

	// If the dash doesn't hit an object head on, but hits it at an angle 
	// greater than MinGlancingHitRedirectAngle from the surface normal,
	// should the dash glance off the object and continue along the surface?
	// Or should it always just impede the dash and cause a bounce?
	// If true, glancing redirection is allowed, otherwise it will just impede
	// the dash.
	// This property is very useful for when you're trying to dash uphill.
	// Instead of dashing straight into the ground and bouncing, you will
	// continue up the slope.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		bool bDoesAllowGlancingHitRedirect = true;

	// If bDoesAllowGlancingHitRedirect is true, this is the minimum angle
	// between a dash and an impact normal. A head on collision is 0 degrees,
	// while a right angle to the surface is 90 degrees.
	// If the calculated angle is greater than this angle, the dash will be
	// redirected along the surface of the impacted object instead of just
	// being impeded and bouncing off.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		float MinGlancingHitRedirectAngle = 55.0f;

	// While GlancingHitRedirect deals with dashing into surfaces at an angle,
	// OffCenterHitSliding deals with not squarely hitting an object with the center
	// of the collision capsule/sphere. If this value is true, then when the Kaiju
	// hits a block and the impact position is not directly in the path of the center
	// of the collision


	// If this value is true, then when the Kaiju hits an object and the impact point
	// is not directly along the path of the center of the collision sphere, the
	// dash will slide along that surface without changing direction.
	// GlancingHitRedirect deals with the angle between a hit surface's normal and the
	// dash direction, while OffCenterHitSliding deals with hitting objects that
	// clip the edge of the collision sphere instead of hitting it in the center.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		bool bDoesAllowOffCenterHitSliding = true;

	// The minimum angle between the Dash's current durection and the contact point
	// for which the hit will slide along the surface instead of glancing or being stopped.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		float MaxOffCenterClipHitSize = 0.3f;
		//float MinOffCenterHitSlidingAngle = 45.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Dash")
		bool bCanChainSuccessfulDashes = true;

	// When the character hits an object which impedes its forward movement, it will
	// bounce off the surface with the velocity it had at the time of impact times this multiplier.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Bounce")
		float BounceVelocityMultiplier = 1.5f;

	// When the character bounces off a surface, its velocity is mirrored/reflected
	// by the surface's normal. In order to add a little vertical motion to every jump
	// you can increase this value. It will add to the vertical velocity of the bump.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Bounce")
		float BounceVerticalVelocityAddition = 100.0f;

	// If true, the bounce velocity will be based on the velocity of the 
	// character during impact, otherwise it is solely based on the BounceVelocityMultiplier
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Bounce")
		bool bIsBounceVelocityBasedOnDashVelocity = false;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnDashStart(FVector DashDirection, int32 ChainedDashCountOfDash);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnDashTick(float DeltaTime, FVector DashDirection);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnDashComplete();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnDashImpede();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnDashHit(bool bWasImpeded);

protected:
	UFUNCTION()
		void Completed();

	UFUNCTION()
		void DashImpeded(FVector CharacterVelocity, FVector ImpactNormal, AActor* HitActor);

	UFUNCTION()
		void DashHit(FVector CharacterVelocity, FVector ImpactNormal, AActor* HitActor);

	UFUNCTION()
		void DashTick(float DeltaTime, FVector DashDirection);

	int32 ChainedDashCount = 0;

	virtual void ResetConsecutiveCount();

	AKaijuCharacter* Character;

	TSet<AActor*> HitActors;
};

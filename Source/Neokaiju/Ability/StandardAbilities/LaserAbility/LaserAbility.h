// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "LaserAbility.generated.h"

class ULaserCameraAbilityTask;
class UParticleSystem;
class USoundBase;
class UCameraShake;
class AKaijuCharacter;

/**
 *
 */
UCLASS()
class NEOKAIJU_API ULaserAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	ULaserAbility();

	// How long it takes for the laser to charge/warm up before it gets fired.
	// During this charging time, ChargeLaserParticleEffect and ChargeLaserSoundEffect will be played.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Charge")
		float ChargeLaserDuration = 0.35f;

	//Whether Kaiju started in air or not.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ability | Charge")
		bool bStartedInAir = false;

	// The effect to apply every LaserDamageRate seconds to whatever is getting hit by the laser.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		TSubclassOf<class UGameplayEffect> LaserDamageEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		TSubclassOf<class UGameplayEffect> LaserOverlapEffect;

	// How often the LaserDamageEffect should be applied to whatever is hit, measured in seconds.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		float LaserDamageRate;

	// How often to subtract the laser's cost, measured in seconds.
	// The actual cost is found in GE_LaserCost.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		float CostCommitRate;

	// How fast the player can change the aim direction of the laser.
	// This is measured in degrees/second. A value of 90 would mean it takes 2
	// seconds for the laser to go from pointed straight up to straight down since 180/90 = 2.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		float LaserAimSpeed;

	// The speed at which the laser initially extends from the character's mouth.
	// A value of 0 instantly extends the laser to MaxLaserDistance.
	// This is measured in units/second. So if MaxLaserDistance is 1000 and LaserExtendSpeed is 500, it will take 2 seconds to reach full length since 1000/500 = 2.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		float LaserExtendSpeed;

	// The maximum distance the laser will extend from the character's mouth if it doesn't hit anything.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		float MaxLaserDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		FName LaserSocketName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		TEnumAsByte<ECollisionChannel> LaserTraceChannel;

	// The radius of the laser's beam when performing collision tests.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		TArray<float> LaserRadiusPerLevel;

	// The distance before and after the laser impact point to apply the impact damage effect.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Fire")
		float LaserImpactRadius;

	// The camera shake to play while the laser is firing.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Camera")
		TSubclassOf<UCameraShake> FireLaserCameraShake;

	// How far the camera should be offset from its default position
	// in the direction the laser is facing
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Camera")
		float LaserCameraOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Camera")
		bool bShouldCenterCameraOnActor;


	UPROPERTY(BlueprintReadOnly)
		AKaijuCharacter* Character;


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	// This function will be implemented in BP by designers to actually spawn effects/sound.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnChargeLaserStart(FName SocketName, USceneComponent* LaserSocketComponent);

	// This function will be implemented in BP by designers to actually spawn effects/sound.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnChargeLaserTick(float TimeRemaining);

	// This function will be implemented in BP by designers to actually spawn effects/sound.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnChargeLaserComplete();

	// This function will be implemented in BP by designers to actually spawn effects/sound.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnChargeLaserCancel();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnFireLaserStart(FName SocketName, USceneComponent* LaserSocketComponent);

	// This function will be implemented in BP by designers to actually spawn effects/sound.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnFireLaserTick(FVector LaserAimDirection, FHitResult LaserHit);

	// This function will be implemented in BP by designers to actually spawn effects/sound.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnFireLaserComplete();

private:
	UFUNCTION()
		void ChargeLaserTick(float TimeRemaining);

	UFUNCTION()
		void ChargeLaserComplete(float TimeRemaining);

	UFUNCTION()
		void ChargeLaserCancel(float TimeRemaining);

	UFUNCTION()
		void FireLaserComplete(FVector LaserAimDirection, FHitResult LaserHit);

	UFUNCTION()
		void FireLaserTick(FVector LaserAimDirection, FHitResult LaserHit);

	ULaserCameraAbilityTask* LaserCameraTask = nullptr;
};

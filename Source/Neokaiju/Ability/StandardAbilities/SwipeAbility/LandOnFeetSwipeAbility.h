// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "LandOnFeetSwipeAbility.generated.h"

class UTickDurationAbilityTask;
class AKaijuCharacter;
class AActor;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API ULandOnFeetSwipeAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULandOnFeetSwipeAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Swipe")
		bool bIsFixedSwipeLength;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Swipe")
		float MaxSwipeDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Swipe")
		bool bShouldDebugDrawPredictedLanding = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Swipe")
		float MinSwipeDegrees;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Swipe")
		bool bDoesFreezeInAir;

	// The gameplay effect that gets applied to actors which are hit by the swipe.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Swipe")
		class TSubclassOf<class UGameplayEffect> SwipeDamageEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnSwipeHit();

protected:
	AKaijuCharacter* Character;

	float SwipeDegreesPerSecond;

	float TotalDegreesRotated;

	FRotator InitialCharacterRotation;
	
	UTickDurationAbilityTask* TickTask;

	UFUNCTION()
		void OnTailHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnSwipeTick(float DeltaTime);

	UFUNCTION()
		void OnSwipeFinished(float DeltaTime);

	bool ShouldStopSwiping();

	void PredictLandingSpot();

	FHitResult PredictedLandingSpot;
	
	TArray<float> PreviousDeltaAngles;

	TSet<AActor*> HitActors;
};

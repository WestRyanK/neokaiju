// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "Neokaiju/Ability/Core/AbilityInputID.h"
#include "CaptureAbility.generated.h"

class ICapturerInterface;
/**
 *
 */

UCLASS()
class NEOKAIJU_API UCaptureAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UCaptureAbility();

	// If this is true, the Captive actor will constantly be moved so its CaptiveComponent
	// is at the same place as the Capturers CapturerComponent. Otherwise, the Captive Actor
	// doesn't get moved at all. See TagOfCapturer/CaptiveComponentToAttach for more info.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsAttachedByComponentTags = true;

	// If this is true, the Captive actor will constantly be moved so it is at the same location
	// relative to the capturer that it was in when it started capturing.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsAttachedByRelativeLocationAtStart = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bShouldDisableCaptiveCollision = true;

	// Instead of just attaching to the root component of the capturer, we can attach to 
	// a specific component of the capturer. The component we attach to is the component which
	// has this tag. If no tag is specified, then we attach to the root component 
	// (if bIsAttachedToCapturer is true).
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString TagOfCapturerComponentToAttach;

	// Instead of just attaching to the root component of the captive active, we can attach to 
	// a specific component. The component we attach to is the component which has this tag.
	// If no tag is specified, we attach to the root component of the captive actor.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString TagOfCaptiveComponentToAttach;

	// The inputID that should be pressed to signal that the captive character is struggling.
	// You should use this instead of AbilityInputID because that is set to activate the ability when the button is
	// pressed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EAbilityInputID StruggleInputID;

	// In order to aggregate on the captive actor the number of struggles required to escape, we use the Stacking functionality of a Gameplay Effect.
	// The Effect set for this property should have Stacking set to Aggregate by Target. The Stack Limit Count will be the maximum number of times
	// the character is required to struggle before being released.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UGameplayEffect> StrugglesToEscapeEffect;

	// Struggle attempts will be ignored when the captured actor has any of these tags applied.
	// This is useful to allow the player to still be captured when dead, but not be able to struggle
	// and escape when dead.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FGameplayTagContainer IgnoreStrugglesWhenCaptiveHasTags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FGameplayTagContainer BlockCapturingWhenCaptiveHasTags;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	// Did capture?
	UFUNCTION(BlueprintCallable)
		static bool CaptureActor(TSubclassOf<UCaptureAbility> CaptureAbilitySubclass, AActor* Capturer, AActor* Captive, int32 NumRequiredStrugglesToEscape);

	UFUNCTION(BlueprintCallable)
		static void ReleaseActor(AActor* Capturer, AActor* Captive);


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SlideToLocationTimeFinished = 0.3;

	// For keeping track of time as we slide.
	float SlideToLocationTimePassed = 0.0;

protected:
	void AddRequiredStrugglesToEscapeToCaptive(int32 Count = 1);

	void RemoveRequiredStrugglesToEscapeToCaptive(int32 Count = 1);

	int32 GetRemainingRequiredStrugglesToEscapeCount() const;

	FActiveGameplayEffectHandle StrugglesToEscapeEffectHandle;

	void InitCaptureFromPayload(const FGameplayEventData* Payload);

	void CheckStrugglesRemaining();

	static UCaptureAbility* GetAbilityInstanceForCapturer(const AActor* CaptiveActor, const AActor* CapturerActor);

	UFUNCTION()
		void OnStrugglePressed();

	UFUNCTION()
		void OnCaptureTick(float DeltaTime);

	UPROPERTY()
		AActor* Capturer;

	UPROPERTY()
		AActor* Captive;

	UPROPERTY()
		USceneComponent* CapturerComponentToAttach;

	UPROPERTY()
		USceneComponent* CaptiveComponentToAttach;

	UPROPERTY()
		int32 NumStrugglesAttempted;

	UPROPERTY()
		int32 NumStrugglesToEscape;

	UPROPERTY()
		float CaptureStartTime;

	UPROPERTY()
		FVector RelativeLocationAtStart;

	UFUNCTION()
		USceneComponent* GetAttachComponent(AActor* Actor, FString ComponentTag);

	UFUNCTION(BlueprintImplementableEvent)
		void CaptureBegin(AActor* CaptiveActor);

	UFUNCTION(BlueprintImplementableEvent)
		void CaptureEnd(AActor* CaptiveActor);
};




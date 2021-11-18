// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "PassivelyDestroyAbility.generated.h"

class AKaijuCharacter;

/**
 *
 */
UCLASS()
class NEOKAIJU_API UPassivelyDestroyAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UPassivelyDestroyAbility();


	// The gameplay effect that gets applied to actors which are touching the KaijuCharacter;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Passively Destroy")
		class TSubclassOf<class UGameplayEffect> PassivelyDestroyEffect;

	// When Actors begin overlapping with the Character, the Ability tries to
	// Passively Destroy them in order to quickly get rid of them.
	// However, there is a case where you level up as you're already overlapping actors.
	// In this case, they will not be Passively Destroyed because the Overlap event
	// already fired. A backup test is in place to occasionally check for
	// Actors which are currently overlapping and try Passively Destroying them.
	// This variable determines how often that check should occur.
	// Since this is an edge case, it doesn't need to run every frame,
	// but a couple times a second is responsive enough.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Passively Destroy")
		float TimeBetweenPassivelyDestroyBackupTests = 0.5f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnPassivelyDestroyed();

protected:
	UPROPERTY()
		AKaijuCharacter* Character;

	UFUNCTION()
		void HitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void BackupTick(int32 ActionNumber);

	bool TryPassivelyDestroy(AActor* ActorToPassivelyDestroy);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Neokaiju/Ability/Core/AbilityInputID.h"
#include "TimerManager.h"
#include "BaseGameplayAbility.generated.h"

#define AB_LOG(Format, ...) \
	if (this->bIsAbilityDebugEnabled) { \
		UE_LOG(LogTemp, Display, Format, ##__VA_ARGS__); \
	}

/**
 *	All abilities should be inherited from this class.
 */
UCLASS()
class NEOKAIJU_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBaseGameplayAbility();

	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability | Base")
		bool bDoesAbilityActivateOnGranted = false;

	// Abilities with this set will automatically activate when the input is pressed
	// if the ability is granted to the character.
	// See AbilityInputID.h for more info about how to use this.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Base")
		EAbilityInputID AbilityInputID = EAbilityInputID::None;

	// If this is enabled, Logging for this ability will be displayed in the console.
	// Otherwise, Logging for this ability is hidden.
	// This is useful so we don't congest the log with output we don't care about.
	// And it is less error prone/less work than commenting out each log statement.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Base")
		bool bIsAbilityDebugEnabled = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Base")
		bool bDoesAbilityRestartWhenCancelled;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Base")
		float RestartAttemptInterval = 0.05f;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	template <typename T>
	static int GetActiveAbilityInstancesOfType(UAbilitySystemComponent* ASC, TArray<T*>& OutActiveAbilityInstancesOfType)
	{
		OutActiveAbilityInstancesOfType.Empty();
		TArray<FGameplayAbilitySpec> Specs = ASC->GetActivatableAbilities();
		for (FGameplayAbilitySpec Spec : Specs)
		{
			if (Spec.Ability->GetClass()->IsChildOf(T::StaticClass()))
			{
				for (UGameplayAbility* AbilityInstance : Spec.GetAbilityInstances())
				{
					OutActiveAbilityInstancesOfType.Add(Cast<T>(AbilityInstance));
				}

			}
		}
		return OutActiveAbilityInstancesOfType.Num();
	}

protected:
	UFUNCTION()
		void RestartAbility();

private:
	FTimerHandle RestartAbilityTimerHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "GameplayEffect.h"
#include "BurningAbility.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UBurningAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UBurningAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// How often the fire should attempt to spread.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Burning")
		float MinTimeBetweenSpreadAttemptsSeconds;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Burning")
		float MaxTimeBetweenSpreadAttemptsSeconds;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Burning")
		float ProbabilityOfSpreading = 0.5f;

	//// The total amount of time to burn.
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Burning")
	//	float BurningTimeSeconds;

	// How far outside the burning actor's bounding box can the fire spread?
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Burning")
		float SpreadDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Burning")
		TSubclassOf<UGameplayEffect> BurnEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Burning")
		TSubclassOf<UGameplayEffect> BurnedEffect;

protected:
	UFUNCTION()
		void SpreadBurn(int32 ActionNumber);

	UFUNCTION()
		void Finished(int32 ActionNumber);
};

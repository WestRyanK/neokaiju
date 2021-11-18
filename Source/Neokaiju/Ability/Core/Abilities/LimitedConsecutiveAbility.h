// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "LimitedConsecutiveAbility.generated.h"

/**
 * Abilities which have a limited number of uses while the character is in the air should inherit from this.
 */
UCLASS()
class NEOKAIJU_API ULimitedConsecutiveAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	// The maximum number of consecutive times this
	// ability can be performed before a certain tag is applied to the owner.
	// For example, if a jump ability has this set to 2,
	// then 2 jumps can be performed while the character is in the air.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Consecutive")
		int32 MaxConsecutiveAbilities = 1;

	// If this is true, then abilities which launch the character into the air
	// count as one of the consecutive uses of the ability in the air.
	// For example, we only want 1 dash to be performable in the air,
	// but if a dash takes the character off the ground and into the air,
	// that should count as the dash performed in the air.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Consecutive")
		bool bDoesCountIncludeWhenOwnerHasResetTags = false;

	// When any of these tags are applied to the owning character, the ConsecutiveAbilitieCount
	// is reset.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Consecutive")
		FGameplayTagContainer ResetConsecutiveAbilitiesTags;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

protected:
	int32 ConsecutiveAbilitiesCount;

	//UFUNCTION()
	//	void OnLanded(const FHitResult& HitResult);

	UFUNCTION()
		void OnTagChanged(const FGameplayTag Tag, int32 TagCount);

	virtual void ResetConsecutiveCount();

	bool DoesOwnerHaveResetTags() const;
};


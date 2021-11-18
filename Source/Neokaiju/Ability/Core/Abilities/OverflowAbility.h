// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "OverflowAbility.generated.h"

class AKaijuCharacter;
/**
 *
 */
UCLASS()
class NEOKAIJU_API UOverflowAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UOverflowAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float OverflowAmount = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<TSubclassOf<UGameplayEffect>> OverflowEffects;

protected:

	UPROPERTY(BlueprintGetter = GetCurrentAmount, BlueprintSetter = SetCurrentAmount)
		float CurrentAmount;

	UFUNCTION(BlueprintGetter)
		float GetCurrentAmount() const;

	UFUNCTION(BlueprintSetter)
		virtual void SetCurrentAmount(float CurrentAmountValue);

	UFUNCTION()
		void AddToCurrentAmount(float AmountToAdd);
};
